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

#include <string.h>
#include <lombok/base/mem.h>
#include <lombok/base/errors.h>
#include <lombok/base/hashCode.h>
#include <lombok/util/charSequence.h>
#include "charBuf.h"

I_REFL_IMPL (charSequence_t, object_t)

charSequence_t::~charSequence_t (void) {
  ___CBTPUSH;

  if (buf != NULL) {
    mem_release (buf);
  }

  ___CBTPOP;
}

uchar_t charSequence_t::charAt (int index) {
  ___CBTPUSH;

  uchar_t result = buf[index];

  ___CBTPOP;
  return result;
}

int charSequence_t::compare (charSequence_t* x, charSequence_t* y) {
  ___CBTPUSH;

  int i = 0;
  int result;
  int max = x->_length < y->_length ? x->_length : y->_length;
  while (i < max && (result = x->buf[i] - y->buf[i]) == 0) {
    ++i;
  }
  if (i == max) {
    result = x->_length - y->_length;
  }

  ___CBTPOP;
  return result;
}

bool charSequence_t::equals (object_t* other) {
  ___CBTPUSH;

  bool result = this == other;
  if (!result) {
    result = classof (charSequence_t)->isAssignableFrom (other->getClass ()) && ((charSequence_t*) other)->_length == _length;
    if (result) {
      result = memcmp (((charSequence_t*) other)->buf, buf, sizeof (uchar_t) * _length) == 0;
    }
  }

  ___CBTPOP;
  return result;
}

unsigned int charSequence_t::hashCode (void) {
  ___CBTPUSH;

  int hash = _length;
  int max = _length < 8 ? _length : 8;
  for (int i=0; i<max; ++i) {
    hash += (int) buf[i] << i;
  }

  ___CBTPOP;
  return hashCode_get (hash);
}

int charSequence_t::length (void) {
  return _length;
}

char* charSequence_t::toUtf8 (char* dst, int allocSizeInBytes, int* bytesWritten) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (allocSizeInBytes < 0) {
    errors_printMessageAndExit ("The value specified for `allocSizeInBytes' should not be negative");
  }
#endif

  charBuf_t buffer (_length + (bytesWritten == NULL), dst, allocSizeInBytes);
  for (int i=0; i<_length; ++i) {
    int val = uchar_toUtf8 (buf[i]);
    while (val > 0) {
      buffer.append (val);
      val >>= 8;
    }
  }
  if (bytesWritten != NULL) {
    bytesWritten[0] = buffer.bytesAppended ();
  } else {
    buffer.append (0);
  }

  ___CBTPOP;
  return buffer.get ();
}
