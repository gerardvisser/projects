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

#include "testInputStream.h"

REFL_IMPL (testInputStream_t, inputStream_t)

testInputStream_t::testInputStream_t (int maxToRead) : m_maxToRead (maxToRead) {
  m_closeWasCalled = false;
  m_byteStream = false;
  m_nextToRead = 0;
  errorCode = 0;
}

int testInputStream_t::close (void) {
  m_closeWasCalled = true;
}

bool testInputStream_t::closeWasCalled (void) {
  return m_closeWasCalled;
}

bool testInputStream_t::isByteStream (void) {
  return m_byteStream;
}

int testInputStream_t::read (void) {
  if (m_nextToRead < m_maxToRead) {
    ++m_nextToRead;
    return m_nextToRead;
  } else {
    return -1;
  }
}

void testInputStream_t::setByteStream (bool val) {
  m_byteStream = val;
}

void testInputStream_t::setErrorCode (long int val) {
  errorCode = val;
}
