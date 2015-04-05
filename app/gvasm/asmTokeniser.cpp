/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2014 Gerard Visser.

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

#include <lombok/base/errors.h>
#include <lombok/parsing/errorCodes.h>
#include <lombok/parsing/tokenisers/secondaryStrings.h>
#include <lombok/parsing/tokenisers/tokens/textToken.h>
#include "asmTokenDataPool.h"
#include "asmTokenIDs.h"
#include "asmTokeniser.h"

REFL_IMPL (asmTokeniser_t, pars::tokeniser_t)

using namespace pars;

asmTokeniser_t::asmTokeniser_t (void) : pars::tokeniser_t (asmTokenDataPool_t::instance ()) {
}

token_t* asmTokeniser_t::createSecondaryToken (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  string_t* text;
  int errorCode = 0;
  int id = BLTYPE_SECONDARY;

  switch (cr) {
  case '%':
    id = TID_S_PERCENT;
    text = get25 ();
    break;

  case '(':
    id = TID_S_PARENTHESIS_OPEN;
    text = get28 ();
    break;

  case ')':
    id = TID_S_PARENTHESIS_CLOSE;
    text = get29 ();
    break;

  case '*':
    id = TID_S_ASTERISK;
    text = get2A ();
    break;

  case '+':
    id = TID_S_PLUS;
    text = get2B ();
    break;

  case ',':
    id = TID_S_COMMA;
    text = get2C ();
    break;

  case '-':
    id = TID_S_MINUS;
    text = get2D ();
    break;

  case '/':
    id = TID_S_SLASH;
    text = get2F ();
    break;

  case ':':
    id = TID_S_COLON;
    text = get3A ();
    break;

  case '[':
    id = TID_S_BRACKET_OPEN;
    text = get5B ();
    break;

  case ']':
    id = TID_S_BRACKET_CLOSE;
    text = get5D ();
    break;

  default:
    errorCode = ERRC_FATAL;
    buffer->clear ();
    buffer->append (cr);
    text = buffer->toString ();
  }

  token_t* token = new textToken_t (id, contextCode, errorCode, line, column, text);
  column += text->length ();
  text->dump ();

  ___CBTPOP;
  return token;
}

token_t* asmTokeniser_t::nextToken (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  token_t* token;
  if (isIdentifierFirstChar (cr)) {

    token = createPrimaryToken (stream, cr);

  } else if (uchar_isDigit (cr)) {

    token = createNumberToken (stream, cr);

  } else {

    switch (cr) {
    case '"':
      token = createStringToken (stream);
      break;

    case '\'':
      token = createCharToken (stream);
      break;

    case '.':
      cr = stream->read ();
      stream->unread (cr);
      if (uchar_isDigit (cr)) {
        token = createNumberToken (stream, '.');
      } else {
        token = createSecondaryToken (stream, '.');
      }
      break;

    case '/':
      cr = stream->read ();
      switch (cr) {
      case '/':
      case '*':
        token = createCommentToken (stream, cr);
        break;

      default:
        stream->unread (cr);
        token = createSecondaryToken (stream, '/');
      }
      break;

    default:
      token = createSecondaryToken (stream, cr);
    }

  }

  ___CBTPOP;
  return token;
}

int asmTokeniser_t::readFloatSuffices (pushbackStream_t* stream, int cr) {
  return cr;
}

int64_t asmTokeniser_t::readHexEscapeSequence (pushbackStream_t* stream, int* errorCode) {
  ___CBTPUSH;

  int cr = stream->read ();
  errorCode[0] = ERRC_FATAL;
  findEndOfCharToken (stream, cr, errorCode);

  ___CBTPOP;
  return 0;
}

int asmTokeniser_t::readIntSuffices (pushbackStream_t* stream, int cr) {
  return cr;
}

int64_t asmTokeniser_t::readNonnumericEscapeSequence (pushbackStream_t* stream, int cr, int* errorCode) {
  ___CBTPUSH;

  int64_t value;
  buffer->append (cr);
  switch (cr) {
  case '"':
  case '\'':
  case '\\': value = cr; break;
  case 'b': value = '\b'; break;
  case 'f': value = '\f'; break;
  case 'n': value = '\n'; break;
  case 'r': value = '\r'; break;
  case 't': value = '\t'; break;
  default: value = 0;
  }
  errorCode[0] = value == 0 ? ERRC_NONFATAL : 0;

  cr = stream->read ();
  if (cr == '\'') {
    buffer->append (cr);
  } else {
    findEndOfCharToken (stream, cr, errorCode);
  }

  ___CBTPOP;
  return value;
}

/* Expects errorCode[0] to be initialised. */
void asmTokeniser_t::validateCharValue (int64_t value, bool wide, int* errorCode) {
  ___CBTPUSH;

  if (errorCode[0] == 0) {
    if (value > 0xFFFF) {
      errorCode[0] = ERRC_NONFATAL;
    }
  }

  ___CBTPOP;
}
