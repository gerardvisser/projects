/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013 - 2014 Gerard Visser.

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
#include <lombok/parsing/contextCodes.h>
#include <lombok/parsing/errorCodes.h>
#include <lombok/parsing/tokenIDs/tokenIDs.h>
#include <lombok/parsing/tokenisers/tokeniser.h>
#include <lombok/parsing/tokenisers/tokens/all.h>

I_REFL_IMPL (pars::tokeniser_t, object_t)

using namespace pars;

tokeniser_t::tokeniser_t (tokenDataPool_t* tokenDataPool) : tokenDataPool (tokenDataPool) {
  ___CBTPUSH;

  buffer = new stringBuilder_t (256);
  tokenDataPool->reserve ();

  ___CBTPOP;
}

tokeniser_t::~tokeniser_t (void) {
  ___CBTPUSH;

  buffer->dump ();
  tokenDataPool->dump ();

  ___CBTPOP;
}

token_t* tokeniser_t::createCharToken (pushbackStream_t* stream, bool wide) {
  ___CBTPUSH;

  buffer->clear ();
  if (wide) {
    buffer->append ('L');
  }
  buffer->append ('\'');

  int64_t value;
  int errorCode = 0;
  int cr = stream->read ();
  if (cr == '\\') {
    buffer->append (cr);
    cr = stream->read ();
    if (cr == -1 || cr == '\n') {
      errorCode = ERRC_FATAL;
      stream->unread (cr);
      value = 0;
    } else if (cr == 'x') {
      buffer->append (cr);
      value = readHexEscapeSequence (stream, &errorCode);
    } else if (cr == 'u') {
      buffer->append (cr);
      value = readUniversalCharacterName (stream, &errorCode);
    } else if (uchar_isOctDigit (cr)) {
      value = readOctalEscapeSequence (stream, cr, &errorCode);
    } else {
      value = readNonnumericEscapeSequence (stream, cr, &errorCode);
    }
    validateCharValue (value, wide, &errorCode);
  } else if (!(cr == -1 || cr == '\n' || cr == '\'')) {
    value = cr;
    buffer->append (cr);
    cr = stream->read ();
    validateCharValue (value, wide, &errorCode);
    if (cr == '\'') {
      buffer->append (cr);
    } else {
      findEndOfCharToken (stream, cr, &errorCode);
    }
  } else if (cr == '\'') {
    errorCode = ERRC_FATAL;
    buffer->append (cr);
    value = 0;
  } else {
    errorCode = ERRC_FATAL;
    stream->unread (cr);
    value = 0;
  }

  string_t* text = buffer->toString ();
  token_t* token = new valueTextToken_t (BLTYPE_CHAR, contextCode, errorCode, line, column, text, value);
  text->dump ();
  column += buffer->length ();

  ___CBTPOP;
  return token;
}

token_t* tokeniser_t::createCommentToken (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;
#ifdef TEST_MODE
# define CREATE_COMMENT_TOKEN_INIT \
  buffer->clear (); \
  buffer->append ('/'); \
  buffer->append (cr);
# define INC_LENGTH buffer->append (cr);
# define NEW_BLOCK_COMMENT_TOKEN(x) \
  string_t* text = buffer->toString (); \
  x =  new multilineTextToken_t (BLTYPE_COMMENT, contextCode, 0, line, column, text, endLine, endColumn); \
  text->dump ();
# define NEW_LINE_COMMENT_TOKEN(x) \
  string_t* text = buffer->toString (); \
  x =  new textToken_t (BLTYPE_COMMENT, contextCode, 0, line, column, text); \
  text->dump ();
#else
# define CREATE_COMMENT_TOKEN_INIT int len = 2;
# define INC_LENGTH ++len;
# define NEW_BLOCK_COMMENT_TOKEN(x) x = new multilineToken_t (BLTYPE_COMMENT, contextCode, 0, line, column, len, endLine, endColumn);
# define NEW_LINE_COMMENT_TOKEN(x) x = new basicToken_t (BLTYPE_COMMENT, contextCode, 0, line, column, len);
#endif

  token_t* token;
  CREATE_COMMENT_TOKEN_INIT;

  if (cr == '*') {

    int endLine = line;
    int endColumn = column + 2;
    bool asterisk = false;
    bool done = false;
    cr = stream->read ();
    while (!(cr == -1 || done)) {
      if (isSpace (cr)) {
        if (cr == 0x0A) {
          endColumn = 1;
          ++endLine;
        } else {
          ++endColumn;
        }
        asterisk = false;
      } else if (cr == '*') {
        asterisk = true;
        ++endColumn;
      } else {
        done = asterisk && cr == '/';
        asterisk = false;
        ++endColumn;
      }
      INC_LENGTH;
      cr = stream->read ();
    }
    stream->unread (cr);
    NEW_BLOCK_COMMENT_TOKEN (token);

  } else {

    cr = stream->read ();
    while (!(cr == -1 || cr == 0x0A)) {
      INC_LENGTH;
      cr = stream->read ();
    }
    stream->unread (cr);
    NEW_LINE_COMMENT_TOKEN (token);

  }

  column = token->endColumn ();
  line = token->endLine ();

  ___CBTPOP;
  return token;
}

token_t* tokeniser_t::createNumberToken (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  buffer->clear ();
  int id;
  int64_t value;
  int errorCode = 0;
  bool firstChar_0 = cr == '0';
  bool firstChar_dot = cr == '.';
  buffer->append (cr);
  cr = stream->read ();
  if (firstChar_0 && (cr == 'x' || cr == 'X')) {

    id = TID_N_HEX;
    value = 0;
    buffer->append (cr);
    cr = stream->read ();
    if (uchar_isHexDigit (cr)) {
      int valueSize = 0;
      int valueSizeStep = 0;
      while (uchar_isHexDigit (cr)) {
        value <<= 4;
        value |= uchar_hexDigitValue (cr);

        valueSizeStep |= cr != '0';
        valueSize += valueSizeStep;

        buffer->append (cr);
        cr = stream->read ();
      }
      if (valueSize > 16) {
        errorCode = ERRC_NONFATAL;
      }
      cr = readIntSuffices (stream, cr);
    } else {
      /* Invalid suffix on integer constant.*/
      errorCode = ERRC_FATAL;
    }

  } else {

    if (cr == '.') {
      id = TID_N_FLOAT;
      buffer->append (cr);
      cr = stream->read ();
    } else {
      id = firstChar_dot ? TID_N_FLOAT : firstChar_0 ? TID_N_OCT : TID_N_DEC;
    }
    while (uchar_isDigit (cr)) {
      buffer->append (cr);
      cr = stream->read ();
    }
    if (id != TID_N_FLOAT && cr == '.') {
      id = TID_N_FLOAT;
      buffer->append (cr);
      cr = stream->read ();
      while (uchar_isDigit (cr)) {
        buffer->append (cr);
        cr = stream->read ();
      }
    }
    if (cr == 'e' || cr == 'E') {
      id = TID_N_FLOAT;
      buffer->append (cr);
      cr = stream->read ();
      if (cr == '-' || cr == '+') {
        buffer->append (cr);
        cr = stream->read ();
      }
      if (!uchar_isDigit (cr)) {
        /* Exponent has no digits. */
        errorCode = ERRC_FATAL;
      }
      while (uchar_isDigit (cr)) {
        buffer->append (cr);
        cr = stream->read ();
      }
    }

    switch (id) {
    case TID_N_DEC:
      value = readDecValue (&errorCode);
      cr = readIntSuffices (stream, cr);
      break;

    case TID_N_FLOAT:
      value = 0;
      cr = readFloatSuffices (stream, cr);
      break;

    case TID_N_OCT:
      value = readOctValue (&errorCode);
      cr = readIntSuffices (stream, cr);
      break;
    }

  }

  if (isIdentifierChar (cr) || cr == '.') {
    while (isIdentifierChar (cr) || cr == '.') {
      buffer->append (cr);
      cr = stream->read ();
    }
    errorCode = ERRC_FATAL;
  }

  stream->unread (cr);

  string_t* text;
  tokenData_t* tokenData = tokenDataPool->get (buffer);
  if (tokenData != NULL) {
    text = tokenData->text;
    text->reserve ();
  } else {
    text = buffer->toString ();
  }

  token_t* token = new valueTextToken_t (id, contextCode, errorCode, line, column, text, value);
  column += text->length ();
  text->dump ();

  ___CBTPOP;
  return token;
}

token_t* tokeniser_t::createPrimaryToken (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  buffer->clear ();
  while (isIdentifierChar (cr)) {
    buffer->append (cr);
    cr = stream->read ();
  }
  stream->unread (cr);

  int id;
  string_t* text;
  tokenData_t* tokenData = tokenDataPool->get (buffer);
  if (tokenData != NULL) {
    text = tokenData->text;
    id = tokenData->id;
    text->reserve ();
  } else {
    text = buffer->toString ();
    id = BLTYPE_PRIMARY;
  }

  token_t* token = new textToken_t (id, contextCode, 0, line, column, text);
  column += text->length ();
  text->dump ();

  ___CBTPOP;
  return token;
}

token_t* tokeniser_t::createStringToken (pushbackStream_t* stream, bool wide) {
  ___CBTPUSH;

  buffer->clear ();
  if (wide) {
    buffer->append ('L');
  }
  buffer->append ('"');

  bool bsl = false;
  int cr = stream->read ();
  while (!(cr == -1 || cr == '\n' || (cr == '"' && !bsl))) {
    bsl = cr == '\\' && !bsl;
    buffer->append (cr);
    cr = stream->read ();
  }

  int errorCode;
  if (cr != '"') {
    errorCode = ERRC_FATAL;
    stream->unread (cr);
  } else {
    errorCode = 0;
    buffer->append (cr);
  }
  string_t* text = buffer->toString ();
  token_t* token = new textToken_t (BLTYPE_STRING, contextCode, errorCode, line, column, text);
  column += text->length ();
  text->dump ();

  ___CBTPOP;
  return token;
}

void tokeniser_t::findEndOfCharToken (pushbackStream_t* stream, int cr, int* errorCode) {
  ___CBTPUSH;

  bool bsl = false;
  while (!(cr == -1 || cr == '\n' || (cr == '\'' && !bsl))) {
    bsl = cr == '\\' && !bsl;
    buffer->append (cr);
    cr = stream->read ();
  }
  if (cr != '\'') {
    errorCode[0] = ERRC_FATAL;
    stream->unread (cr);
  } else {
    if (errorCode[0] < ERRC_NONFATAL) {
      errorCode[0] = ERRC_NONFATAL;
    }
    buffer->append (cr);
  }

  ___CBTPOP;
}

arrayList_t* tokeniser_t::getTokens (pushbackStream_t* stream) {
  ___CBTPUSH;

  resetState ();
  int cr = stream->read ();
  arrayList_t* list = new arrayList_t (0x1000);
  while (cr != -1) {
    if (isSpace (cr)) {
      cr = skipSpaces (stream, cr);
    } else {
      list->append (nextToken (stream, cr));
      cr = stream->read ();
      contextCode = 0;
    }
  }
  list->append (new basicToken_t (0, CTXT_LAST_TOKEN, 0, line, column, 0));
  list->trimToSize ();

  ___CBTPOP;
  return list;
}

bool tokeniser_t::isIdentifierChar (int cr) {
  return isIdentifierFirstChar (cr) || uchar_isDigit (cr);
}

bool tokeniser_t::isIdentifierFirstChar (int cr) {
  return (cr > 0x40 && cr < 0x5B) || (cr > 0x60 && cr < 0x7B) || cr == '_';
}

bool tokeniser_t::isSpace (int cr) {
  return cr == ' ' || cr == '\t' || cr == '\n' || cr == 0xA0 || cr == '\r';
}

int64_t tokeniser_t::readDecValue (int* errorCode) {
  ___CBTPUSH;

  buffer->append ((uchar_t) 0);

  int index = 0;
  int64_t value = 0;
  int cr = buffer->charAt (index);
  bool firstChar_1 = cr == '1';
  while (uchar_isDigit (cr)) {
    value *= 10;
    value += cr - 0x30;
    ++index;
    cr = buffer->charAt (index);
  }
  if (index == 20 && firstChar_1) {
    if ((value & 0x8000000000000000) == 0) {
      errorCode[0] = ERRC_NONFATAL;
    }
  } else if (index > 19) {
    errorCode[0] = ERRC_NONFATAL;
  }

  buffer->removeLastCharacter ();

  ___CBTPOP;
  return value;
}

int64_t tokeniser_t::readOctalEscapeSequence (pushbackStream_t* stream, int cr, int* errorCode) {
  ___CBTPUSH;

  int len = 0;
  int64_t value = 0;
  while (uchar_isOctDigit (cr)) {
    value <<= 3;
    value |= cr - 0x30;
    buffer->append (cr);
    cr = stream->read ();
    ++len;
  }
  errorCode[0] = len > 3 ? ERRC_NONFATAL : 0;

  if (cr == '\'') {
    buffer->append (cr);
  } else {
    findEndOfCharToken (stream, cr, errorCode);
  }

  ___CBTPOP;
  return value;
}

int64_t tokeniser_t::readOctValue (int* errorCode) {
  ___CBTPUSH;

  buffer->append ((uchar_t) 0);

  int index = 0;
  int64_t value = 0;
  while (buffer->charAt (index) == '0') {
    ++index;
  }
  if (buffer->charAt (index) == '1') {
    value = 1;
    ++index;
  }
  int valueSizeStartIndex = index;
  int cr = buffer->charAt (index);
  while (uchar_isOctDigit (cr)) {
    value <<= 3;
    value |= cr - 0x30;
    ++index;
    cr = buffer->charAt (index);
  }
  if (cr == 0) {
    if (index - valueSizeStartIndex > 21) {
      errorCode[0] = ERRC_NONFATAL;
    }
  } else {
    /* Invalid digit in octal constant. */
    errorCode[0] = ERRC_FATAL;
  }

  buffer->removeLastCharacter ();

  ___CBTPOP;
  return value;
}

int64_t tokeniser_t::readUniversalCharacterName (pushbackStream_t* stream, int* errorCode) {
  ___CBTPUSH;

  int len = 0;
  int64_t value = 0;
  int cr = stream->read ();
  while (uchar_isHexDigit (cr)) {
    value <<= 4;
    value |= uchar_hexDigitValue (cr);
    buffer->append (cr);
    cr = stream->read ();
    ++len;
  }
  if (len == 4) {
    errorCode[0] = 0;
  } else if (len < 4) {
    /* Incomplete universal character name. */
    errorCode[0] = ERRC_FATAL;
  } else {
    errorCode[0] = ERRC_NONFATAL;
  }

  if (cr == '\'') {
    buffer->append (cr);
  } else {
    findEndOfCharToken (stream, cr, errorCode);
  }

  ___CBTPOP;
  return value;
}

void tokeniser_t::resetState (void) {
  ___CBTPUSH;

  contextCode = CTXT_FIRST_TOKEN;
  column = 1;
  line = 1;

  ___CBTPOP;
}

int tokeniser_t::skipSpaces (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  while (isSpace (cr)) {
    if (cr == 0x0A) {
      contextCode |= CTXT_NEW_LINE_BEFORE;
      column = 1;
      ++line;
    } else {
      contextCode |= CTXT_WHITESPACE_BEFORE;
      ++column;
    }
    cr = stream->read ();
  }

  ___CBTPOP;
  return cr;
}
