/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2012, 2013, 2014 Gerard Visser.

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

#ifndef IO_BUF_SIZE
# define IO_BUF_SIZE 0x1000
#endif

#include <string.h>
#include <unistd.h>
#include <lombok/base/mem.h>
#include <lombok/base/errors.h>
#include <lombok/io/errorCodes.h>
#include <lombok/io/fileInputStream.h>

REFL_IMPL (fileInputStream_t, inputStream_t)

fileInputStream_t::fileInputStream_t (const char* fileName) {
  ___CBTPUSH;

  initialise (fileName);

  ___CBTPOP;
}

fileInputStream_t::fileInputStream_t (charSequence_t* fileName) {
  ___CBTPUSH;

  int bufSize = fileName->length () + 1;
  char buf[bufSize];
  char* utf8 = fileName->toUtf8 (buf, bufSize);
  initialise (utf8);
  if (utf8 != buf) {
    mem_release (utf8);
  }

  ___CBTPOP;
}

fileInputStream_t::~fileInputStream_t (void) {
  ___CBTPUSH;

  close ();

  ___CBTPOP;
}

int fileInputStream_t::close (void) {
  ___CBTPUSH;

  if (m_handle != NULL) {
    mem_release (m_buf);
    fclose (m_handle);
    m_handle = NULL;
  }

  ___CBTPOP;
  return 0;
}

void fileInputStream_t::initialise (const char* fileName) {
  ___CBTPUSH;

  if (access (fileName, F_OK)) {

    errorCode = IO_ERR__NO_SUCH_FILE;
    m_handle = NULL;
    m_buf = NULL;
    m_size = 0;

  } else if (access (fileName, R_OK)) {

    errorCode = IO_ERR__NO_READ_PERMISSION;
    m_handle = NULL;
    m_buf = NULL;
    m_size = 0;

  } else {

    errorCode = 0;
    m_handle = fopen (fileName, "rb");
    m_buf = (unsigned char*) mem_allocate ("char[]", IO_BUF_SIZE, PTYPE_INTERNAL);
    m_size = fread (m_buf, 1, IO_BUF_SIZE, m_handle);

  }
  m_index = 0;

  ___CBTPOP;
}

bool fileInputStream_t::isByteStream (void) {
  return true;
}

int fileInputStream_t::read (void) {
  ___CBTPUSH;

  int result;
  if (m_index == IO_BUF_SIZE) {
    m_size = fread (m_buf, 1, IO_BUF_SIZE, m_handle);
    m_index = 0;
  }
  if (m_index < m_size) {
    result = m_buf[m_index];
    ++m_index;
  } else {
    result = -1;
  }

  ___CBTPOP;
  return result;
}

int fileInputStream_t::read (unsigned char* dst, int count) {
  ___CBTPUSH;

  int numberOfBytesRead;
  if (count > 0) {
    const int buffered = m_size - m_index;
    if (count > buffered) {
      memcpy (dst, m_buf + m_index, buffered);
      numberOfBytesRead = buffered;
      m_index += buffered;
      if (m_index == IO_BUF_SIZE) {
        numberOfBytesRead += fread (dst + buffered, 1, count - buffered, m_handle);
      }
    } else {
      memcpy (dst, m_buf + m_index, count);
      numberOfBytesRead = count;
      m_index += count;
    }
  } else {
    numberOfBytesRead = 0;
  }

  ___CBTPOP;
  return numberOfBytesRead;
}
