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

#include <string.h>
#include <lombok/base/errors.h>
#include <lombok/base/mem.h>
#include <lombok/util/stringBuilder.h>

REFL_IMPL (stringBuilder_t, charSequence_t)

stringBuilder_t::stringBuilder_t (int initialCapacity) {
  ___CBTPUSH;

  buf = (uchar_t*) mem_allocate ("uchar_t[]", sizeof (uchar_t) * initialCapacity, PTYPE_INTERNAL);
  m_capacityQuantum = initialCapacity;
  m_capacity = initialCapacity;
  _length = 0;

  ___CBTPOP;
}

void stringBuilder_t::append (uchar_t val) {
  ___CBTPUSH;

  ensureCapacity (1);
  buf[_length] = val;
  ++_length;

  ___CBTPOP;
}

void stringBuilder_t::append (uchar_t val, int numberOfTimes) {
  ___CBTPUSH;

  ensureCapacity (numberOfTimes);
  const int max = _length + numberOfTimes;
  while (_length < max) {
    buf[_length] = val;
    ++_length;
  }

  ___CBTPOP;
}

void stringBuilder_t::append (charSequence_t* val) {
  ___CBTPUSH;

  if (val->_length > 0) {
    ensureCapacity (val->_length);
    memcpy (buf + _length, val->buf, val->_length * sizeof (uchar_t));
    _length += val->_length;
  }

  ___CBTPOP;
}

void stringBuilder_t::clear (void) {
  ___CBTPUSH;

  _length = 0;

  ___CBTPOP;
}

void stringBuilder_t::ensureCapacity (int additionalLength) {
  ___CBTPUSH;

  long int delta = (long int) _length + additionalLength - m_capacity;
  if (delta > 0) {
    delta = 1 + (delta / m_capacityQuantum);
    delta *= m_capacityQuantum;
    m_capacity += delta;
    buf = (uchar_t*) mem_reallocate (buf, m_capacity * sizeof (uchar_t));
  }

  ___CBTPOP;
}

void stringBuilder_t::removeLastCharacter (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (_length == 0) {
    errors_printMessageAndExit ("Cannot remove characters from an empty string");
  }
#endif

  --_length;

  ___CBTPOP;
}

string_t* stringBuilder_t::toString (void) {
  ___CBTPUSH;

  string_t* result = new string_t (buf, _length);

  ___CBTPOP;
  return result;
}
