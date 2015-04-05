/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013 Gerard Visser.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <string.h>
#include <lombok/base/mem.h>
#include <lombok/base/errors.h>
#include <lombok/codepainter/codepainter.h>
#include <lombok/io/fileOutputStream.h>
#include <lombok/parsing/contextCodes.h>
#include <lombok/parsing/tokenCodes.h>

static const char* const htmlBegin = "<!DOCTYPE html>\n<html>\n  <head>\n    <meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n    <style type=\"text/css\">\n      body {\n        background-color: #000000;\n        color: #FFFFFF;\n      }\n      pre {\n        font-size: 16px;\n      }\n      span.comment {\n        color: #FF7F24;\n      }\n      span.error {\n        border-bottom: 1px solid #FF0000;\n      }\n      span.func {\n        color: #87CEFA;\n      }\n      span.keyword {\n        color: #00FFFF;\n      }\n      span.namespace {\n        color: #7FFFD4;\n      }\n      span.pre {\n        color: #B0C4DE;\n      }\n      span.string {\n        color: #FFA07A;\n      }\n      span.type {\n        color: #98FB98;\n      }\n      span.var {\n        color: #EEDD82;\n      }\n      span.warn {\n        border-bottom: 1px solid #FFE000;\n      }\n    </style>\n  </head>\n  <body>\n    <pre>";
static const char* const htmlEnd = "</pre>\n  </body>\n</html>\n";
static const char* const spanClose = "</span>";

typedef struct {
  int column;
  int line;
  int style;
} context_t;

static void writeToken (fileOutputStream_t* fos, context_t* context, pars::token_t* token, int style);

int codepainter_colourCode (pars::token_t* token) {
  ___BTPUSH;

  int result = token->code ();

  ___BTPOP;
  return result;
}

void codepainter_createHtml (const char* fileName, list_t* tokens, int (* colourCode)(pars::token_t* token)) {
  ___BTPUSH;

  fileOutputStream_t* fos = new fileOutputStream_t (fileName);
  fos->write (htmlBegin);

  context_t context = {1, 1, 0};
  listIterator_t* iter = tokens->iterator ();
  pars::token_t* token = (pars::token_t*) iter->next ();
  while (token->contextCode () != CTXT_LAST_TOKEN) {
    writeToken (fos, &context, token, colourCode (token));
    token = (pars::token_t*) iter->next ();
  }
  iter->dump ();

  if (context.style != 0) {
    fos->write (spanClose);
  }
  fos->write (htmlEnd);
  fos->dump ();

  ___BTPOP;
}

static void writeSpaces (fileOutputStream_t* fos, context_t* context, pars::token_t* token) {
  ___BTPUSH;

  int i;
  int n = token->beginLine () - context->line;
  if (n > 0) {
    context->column = 1;
    for (i = 0; i < n; ++i) {
      fos->write ('\n');
    }
  }
  n = token->beginColumn () - context->column;
  for (i = 0; i < n; ++i) {
    fos->write (' ');
  }

  ___BTPOP;
}

static void writeSpanOpen (fileOutputStream_t* fos, int style) {
  ___BTPUSH;

  const int underline = style & 3;
  const int colour = style & 0xFFFFFFFC;
  fos->write ("<span class=\"");
  switch (colour) {
  case TCODE_COMMENT: fos->write ("comment"); break;
  case TCODE_FUNC: fos->write ("func"); break;
  case TCODE_KEYWORD: fos->write ("keyword"); break;
  case TCODE_NAMESPACE: fos->write ("namespace"); break;
  case TCODE_PRE: fos->write ("pre"); break;
  case TCODE_STRING: fos->write ("string"); break;
  case TCODE_TYPE: fos->write ("type"); break;
  case TCODE_VAR: fos->write ("var");
  }
  if (underline != 0) {
    if (colour != 0) {
      fos->write (' ');
    }
    if (underline == TCODE_ERROR) {
      fos->write ("error");
    } else {
      fos->write ("warn");
    }
  }
  fos->write ("\">");

  ___BTPOP;
}

static void writeText (fileOutputStream_t* fos, pars::token_t* token) {
  ___BTPUSH;

  string_t* text = token->text ();
  if (text != NULL) {
    char buf[256];
    char* utf8 = text->toUtf8 (buf, 256);
    int len = strlen (utf8);
    for (int i = 0; i < len; ++i) {
      switch (utf8[i]) {
      case '&':
        fos->write ("&#x26;");
        break;
      case '<':
        fos->write ("&#x3C;");
        break;
      case '>':
        fos->write ("&#x3E;");
        break;
      default:
        fos->write (utf8[i]);
      }
    }
    if (utf8 != buf) {
      mem_release (utf8);
    }
  } else {
    /* What to do here? */
  }

  ___BTPOP;
}

static void writeToken (fileOutputStream_t* fos, context_t* context, pars::token_t* token, int style) {
  ___BTPUSH;

  if (context->style != style) {
    if (context->style != 0) {
      fos->write (spanClose);
      if ((style & 3) == 0 || (context->style & 3) != (style & 3)) {
        writeSpaces (fos, context, token);
        if (style != 0) {
          writeSpanOpen (fos, style);
        }
      } else {
        writeSpanOpen (fos, style);
        writeSpaces (fos, context, token);
      }
    } else {
      writeSpaces (fos, context, token);
      writeSpanOpen (fos, style);
    }
  } else {
    writeSpaces (fos, context, token);
  }
  writeText (fos, token);
  context->column = token->endColumn ();
  context->line = token->endLine ();
  context->style = style;

  ___BTPOP;
}
