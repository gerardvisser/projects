/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013, 2014 Gerard Visser.

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
#include <lombok/parsing/tokenIDs/tokenIDs.h>
#include <lombok/parsing/tokenisers/javaTokeniser.h>
#include <lombok/parsing/tokenisers/secondaryStrings.h>
#include <lombok/parsing/tokenisers/tokens/textToken.h>
#include "javaTokenDataPool.h"

REFL_IMPL (pars::javaTokeniser_t, pars::tokeniser_t)

using namespace pars;

javaTokeniser_t::javaTokeniser_t (void) : tokeniser_t (javaTokenDataPool_t::instance ()) {
  ___CBTPUSH;
  ___CBTPOP;
}

token_t* javaTokeniser_t::createSecondaryToken (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  string_t* text;
  int errorCode = 0;
  int id = BLTYPE_SECONDARY;

  switch (cr) {
  case '!':
    cr = stream->read ();
    if (cr == '=') {
      id = TID_S_NOT_EQUAL;
      text = get213D ();
    } else {
      id = TID_S_EXCLAMATION_MARK;
      text = get21 ();
      stream->unread (cr);
    }
    break;

  case '#':
    id = TID_S_HASH;
    errorCode = ERRC_FATAL;
    text = get23 ();
    break;

  case '%':
    cr = stream->read ();
    if (cr == '=') {
      id = TID_S_MOD;
      text = get253D ();
    } else {
      id = TID_S_PERCENT;
      text = get25 ();
      stream->unread (cr);
    }
    break;

  case '&':
    cr = stream->read ();
    switch (cr) {
    case '&':
      id = TID_S_LOG_AND;
      text = get2626 ();
      break;

    case '=':
      id = TID_S_AND;
      text = get263D ();
      break;

    default:
      id = TID_S_AMPERSAND;
      text = get26 ();
      stream->unread (cr);
    }
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
    cr = stream->read ();
    if (cr == '=') {
      id = TID_S_MUL;
      text = get2A3D ();
    } else {
      id = TID_S_ASTERISK;
      text = get2A ();
      stream->unread (cr);
    }
    break;

  case '+':
    cr = stream->read ();
    switch (cr) {
    case '+':
      id = TID_S_INC;
      text = get2B2B ();
      break;

    case '=':
      id = TID_S_ADD;
      text = get2B3D ();
      break;

    default:
      id = TID_S_PLUS;
      text = get2B ();
      stream->unread (cr);
    }
    break;

  case ',':
    id = TID_S_COMMA;
    text = get2C ();
    break;

  case '-':
    cr = stream->read ();
    switch (cr) {
    case '-':
      id = TID_S_DEC;
      text = get2D2D ();
      break;

    case '=':
      id = TID_S_SUB;
      text = get2D3D ();
      break;

    default:
      id = TID_S_MINUS;
      text = get2D ();
      stream->unread (cr);
    }
    break;

  case '.':
    cr = stream->read ();
    if (cr == '.') {

      cr = stream->read ();
      if (cr == '.') {
        id = TID_S_ELLIPSIS;
        text = get2E2E2E ();
      } else {
        errorCode = ERRC_FATAL;
        text = get2E2E ();
        stream->unread (cr);
      }

    } else {
      id = TID_S_DOT;
      text = get2E ();
      stream->unread (cr);
    }
    break;

  case '/':
    cr = stream->read ();
    if (cr == '=') {
      id = TID_S_DIV;
      text = get2F3D ();
    } else {
      id = TID_S_SLASH;
      text = get2F ();
      stream->unread (cr);
    }
    break;

  case ':':
    id = TID_S_COLON;
    text = get3A ();
    break;

  case ';':
    id = TID_S_SEMICOLON;
    text = get3B ();
    break;

  case '<':
    cr = stream->read ();
    switch (cr) {
    case '<':
      cr = stream->read ();
      if (cr == '=') {
        id = TID_S_SAL;
        text = get3C3C3D ();
      } else {
        id = TID_S_MUCH_LESS;
        text = get3C3C ();
        stream->unread (cr);
      }
      break;

    case '=':
      id = TID_S_NOT_GREATER;
      text = get3C3D ();
      break;

    default:
      id = TID_S_LESS;
      text = get3C ();
      stream->unread (cr);
    }
    break;

  case '=':
    cr = stream->read ();
    if (cr == '=') {
      id = TID_S_EQUAL;
      text = get3D3D ();
    } else {
      id = TID_S_MOV;
      text = get3D ();
      stream->unread (cr);
    }
    break;

  case '>':
    cr = stream->read ();
    switch (cr) {
    case '>':
      cr = stream->read ();
      switch (cr) {
      case '>':
        cr = stream->read ();
        if (cr == '=') {
          id = TID_S_SHR;
          text = get3E3E3E3D ();
        } else {
          id = TID_S_SHRWOA;
          text = get3E3E3E ();
          stream->unread (cr);
        }
        break;

      case '=':
        id = TID_S_SAR;
        text = get3E3E3D ();
        break;

      default:
        id = TID_S_MUCH_GREATER;
        text = get3E3E ();
        stream->unread (cr);
      }
      break;

    case '=':
      id = TID_S_NOT_LESS;
      text = get3E3D ();
      break;

    default:
      id = TID_S_GREATER;
      text = get3E ();
      stream->unread (cr);
    }
    break;

  case '?':
    id = TID_S_QUESTION_MARK;
    text = get3F ();
    break;

  case '@':
    id = TID_S_AT;
    text = get40 ();
    break;

  case '[':
    id = TID_S_BRACKET_OPEN;
    text = get5B ();
    break;

  case '\\':
    id = TID_S_BACKSLASH;
    errorCode = ERRC_FATAL;
    text = get5C ();
    break;

  case ']':
    id = TID_S_BRACKET_CLOSE;
    text = get5D ();
    break;

  case '^':
    cr = stream->read ();
    if (cr == '=') {
      id = TID_S_XOR;
      text = get5E3D ();
    } else {
      id = TID_S_HAT;
      text = get5E ();
      stream->unread (cr);
    }
    break;

  case '{':
    id = TID_S_BRACE_OPEN;
    text = get7B ();
    break;

  case '|':
    cr = stream->read ();
    switch (cr) {
    case '=':
      id = TID_S_OR;
      text = get7C3D ();
      break;

    case '|':
      id = TID_S_LOG_OR;
      text = get7C7C ();
      break;

    default:
      id = TID_S_PIPE;
      text = get7C ();
      stream->unread (cr);
    }
    break;

  case '}':
    id = TID_S_BRACE_CLOSE;
    text = get7D ();
    break;

  case '~':
    id = TID_S_TILDE;
    text = get7E ();
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

bool javaTokeniser_t::isIdentifierFirstChar (int cr) {
  return (cr > 0x40 && cr < 0x5B) || (cr > 0x60 && cr < 0x7B) || cr == '_' || cr == '$';
}

token_t* javaTokeniser_t::nextToken (pushbackStream_t* stream, int cr) {
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

int javaTokeniser_t::readFloatSuffices (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  if (cr == 'f' || cr == 'F' || cr == 'd' || cr == 'D') {
    buffer->append (cr);
    cr = stream->read ();
  }

  ___CBTPOP;
  return cr;
}

int64_t javaTokeniser_t::readHexEscapeSequence (pushbackStream_t* stream, int* errorCode) {
  ___CBTPUSH;

  int cr = stream->read ();
  errorCode[0] = ERRC_FATAL;
  findEndOfCharToken (stream, cr, errorCode);

  ___CBTPOP;
  return 0;
}

int javaTokeniser_t::readIntSuffices (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  if (cr == 'L' || cr == 'l') {
    buffer->append (cr);
    cr = stream->read ();
  }

  ___CBTPOP;
  return cr;
}

int64_t javaTokeniser_t::readNonnumericEscapeSequence (pushbackStream_t* stream, int cr, int* errorCode) {
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
void javaTokeniser_t::validateCharValue (int64_t value, bool wide, int* errorCode) {
  ___CBTPUSH;

  if (errorCode[0] == 0) {
    if (value > 0xFFFF) {
      errorCode[0] = ERRC_NONFATAL;
    }
  }

  ___CBTPOP;
}
