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
#include <lombok/base/mem.h>
#include <lombok/io/fileInputStream.h>
#include "interruptService.h"

#define BLOCK_SIZE 0x4000

REFL_IMPL (interruptService_t, object_t)

static int readWord (fileInputStream_t* fis);

interruptService_t::interruptService_t (const char* filename) {
  ___CBTPUSH;

  m_data = NULL;
  m_dataSize = 0;
  fileInputStream_t* fis = new fileInputStream_t (filename);
  m_errors = fis->getErrorCode () != 0;
  if (!m_errors) {
    m_errors |= fis->read () != 'X';
    m_errors |= fis->read () != 'F';
    m_errors |= fis->read () != 10;
    m_errors |= fis->read () != 0;
    m_serviceNumber = readWord (fis);
    m_ipStart = readWord (fis);
    m_csStart = readWord (fis);
    m_errors = m_serviceNumber > 0xFF;
    if (!m_errors) {
      m_data = (unsigned char*) mem_allocate ("char[]", BLOCK_SIZE, PTYPE_INTERNAL);
      int bytesRead = fis->read (m_data, BLOCK_SIZE);
      m_dataSize += bytesRead;
      while (bytesRead == BLOCK_SIZE) {
        m_data = (unsigned char*) mem_reallocate (m_data, m_dataSize + BLOCK_SIZE);
        bytesRead = fis->read (m_data + m_dataSize, BLOCK_SIZE);
        m_dataSize += bytesRead;
      }
    }
  }
  fis->dump ();

  ___CBTPOP;
}

interruptService_t::~interruptService_t (void) {
  ___CBTPUSH;

  if (m_data != NULL) {
    mem_release (m_data);
  }

  ___CBTPOP;
}

int interruptService_t::csStart (void) {
  return m_csStart;
}

const unsigned char* interruptService_t::data (void) {
  return m_data;
}

int interruptService_t::dataSize (void) {
  return m_dataSize;
}

bool interruptService_t::hasErrors (void) {
  return m_errors;
}

int interruptService_t::ipStart (void) {
  return m_ipStart;
}

int interruptService_t::serviceNumber (void) {
  return m_serviceNumber;
}

static int readWord (fileInputStream_t* fis) {
  ___BTPUSH;

  int lb = fis->read ();
  int result = fis->read () << 8 | lb;

  ___BTPOP;
  return result;
}
