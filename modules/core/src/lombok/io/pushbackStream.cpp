/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2012, 2013 Gerard Visser.

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
#include <lombok/io/pushbackStream.h>

REFL_IMPL (pushbackStream_t, inputStream_t)

pushbackStream_t::pushbackStream_t (inputStream_t* stream) : stream (stream) {
  ___CBTPUSH;

  buffer = -1;

  ___CBTPOP;
}

pushbackStream_t::~pushbackStream_t (void) {
  ___CBTPUSH;

  stream->dump ();

  ___CBTPOP;
}

int pushbackStream_t::close (void) {
  ___CBTPUSH;

  stream->close ();

  ___CBTPOP;
}

long int pushbackStream_t::getErrorCode (void) {
  ___CBTPUSH;

  long result = stream->getErrorCode ();

  ___CBTPOP;
  return result;
}

bool pushbackStream_t::isByteStream (void) {
  ___CBTPUSH;

  bool result = stream->isByteStream ();

  ___CBTPOP;
  return result;
}

int pushbackStream_t::read (void) {
  ___CBTPUSH;

  int result;
  if (buffer == -1) {
    result = stream->read ();
  } else {
    result = buffer;
    buffer = -1;
  }

  ___CBTPOP;
  return result;
}

void pushbackStream_t::unread (int val) {
  ___CBTPUSH;

  if (getErrorCode () == 0) {
    buffer = val;
  }

  ___CBTPOP;
}
