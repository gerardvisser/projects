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
#include <lombok/parsing/tokenisers/abstractCTokeniser.h>

I_REFL_IMPL (pars::abstractCTokeniser_t, pars::tokeniser_t)

using namespace pars;

abstractCTokeniser_t::abstractCTokeniser_t (tokenDataPool_t* tokenDataPool) : tokeniser_t (tokenDataPool) {
}

token_t* abstractCTokeniser_t::nextToken (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  token_t* token;
  if (isIdentifierFirstChar (cr)) {

    if (cr == 'L') {
      cr = stream->read ();
      switch (cr) {
      case '\'':
        token = createCharToken (stream, true);
        break;

      case '"':
        token = createStringToken (stream, true);
        break;

      default:
        stream->unread (cr);
        token = createPrimaryToken (stream, 'L');
      }
    } else {
      token = createPrimaryToken (stream, cr);
    }

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

int abstractCTokeniser_t::readFloatSuffices (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  if (cr == 'f' || cr == 'F' || cr == 'L' || cr == 'l' || cr == 'd' || cr == 'D') {
    buffer->append (cr);
    cr = stream->read ();
  }

  ___CBTPOP;
  return cr;
}

int64_t abstractCTokeniser_t::readHexEscapeSequence (pushbackStream_t* stream, int* errorCode) {
  ___CBTPUSH;

  int64_t value = 0;
  int cr = stream->read ();
  while (uchar_isHexDigit (cr)) {
    value <<= 4;
    value |= uchar_hexDigitValue (cr);
    buffer->append (cr);
    cr = stream->read ();
  }
  errorCode[0] = 0;

  if (cr == '\'') {
    buffer->append (cr);
  } else {
    findEndOfCharToken (stream, cr, errorCode);
  }

  ___CBTPOP;
  return value;
}

int abstractCTokeniser_t::readIntSuffices (pushbackStream_t* stream, int cr) {
  ___CBTPUSH;

  if (cr == 'L' || cr == 'l') { /* L */

    buffer->append (cr);
    cr = stream->read ();
    if (cr == 'L' || cr == 'l') { /* LL */
      buffer->append (cr);
      cr = stream->read ();
      if (cr == 'U' || cr == 'u') { /* LLU */
        buffer->append (cr);
        cr = stream->read ();
      }
    } else if (cr == 'U' || cr == 'u') { /* LU */
      buffer->append (cr);
      cr = stream->read ();
    }

  } else if (cr == 'U' || cr == 'u') { /* U */

    buffer->append (cr);
    cr = stream->read ();
    if (cr == 'L' || cr == 'l') { /* UL */
      buffer->append (cr);
      cr = stream->read ();
      if (cr == 'L' || cr == 'l') { /* ULL */
        buffer->append (cr);
        cr = stream->read ();
      }
    }

  }

  ___CBTPOP;
  return cr;
}

int64_t abstractCTokeniser_t::readNonnumericEscapeSequence (pushbackStream_t* stream, int cr, int* errorCode) {
  ___CBTPUSH;

  int64_t value;
  buffer->append (cr);
  switch (cr) {
  case '"':
  case '\'':
  case '?':
  case '\\': value = cr; break;
  case 'a': value = '\a'; break;
  case 'b': value = '\b'; break;
  case 'f': value = '\f'; break;
  case 'n': value = '\n'; break;
  case 'r': value = '\r'; break;
  case 't': value = '\t'; break;
  case 'v': value = '\v'; break;
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
void abstractCTokeniser_t::validateCharValue (int64_t value, bool wide, int* errorCode) {
  ___CBTPUSH;

  if (errorCode[0] == 0) {
    if (wide) {
      if (value > 0x7FFFFFFF) {
        errorCode[0] = ERRC_NONFATAL;
      }
    } else if (value > 0xFF) {
      errorCode[0] = ERRC_NONFATAL;
    }
  }

  ___CBTPOP;
}
