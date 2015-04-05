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

#include <ctype.h>
#include <stdio.h>
#include <lombok/base/errors.h>
#include <lombok/base/mem.h>
#include "lineInputReader.h"

REFL_IMPL (lineInputReader_t, object_t)

lineInputReader_t::lineInputReader_t (void) {
  ___CBTPUSH;

  m_size = 256;
  m_position = 0;
  m_charsRead = 0;
  m_buffer = (char*) mem_allocate ("char[]", m_size, PTYPE_INTERNAL);

  ___CBTPOP;
}

lineInputReader_t::~lineInputReader_t (void) {
  ___CBTPUSH;

  mem_release (m_buffer);

  ___CBTPOP;
}

void lineInputReader_t::getLineFromStdin (void) {
  ___CBTPUSH;

  size_t size = m_size;
  getline (&m_buffer, &size, stdin);
  if (size > m_size) {
    /* TODO: update memory administration: a realloc occurred. */
    m_size = size;
  }
  m_position = 0;
  m_charsRead = 0;
  while (isspace (m_buffer[m_position])) {
    ++m_position;
  }
  m_startPosition = m_position;

  ___CBTPOP;
}

int lineInputReader_t::position (void) {
  return m_position;
}

int lineInputReader_t::read (bool convertToUpper) {
  ___CBTPUSH;

  int result = m_buffer[m_position];
  if (result != 0) {
    ++m_position;
    ++m_charsRead;
    if (isspace (result)) {
      while (isspace (m_buffer[m_position])) {
        ++m_position;
      }
      result = m_buffer[m_position] != 0 ? ' ' : 0;
    } else if (convertToUpper) {
      result = toupper (result);
    }
  }

  ___CBTPOP;
  return result;
}

char* lineInputReader_t::readAll (char* buf, int* bufSize, bool convertToUpper) {
  ___CBTPUSH;

  int i = 0;
  int c = read (convertToUpper);
  while (c != 0) {
    if (i == bufSize[0]) {
      bufSize[0] += 256;
      buf = (char*) mem_reallocate (buf, bufSize[0]);
    }
    buf[i] = c;
    c = read (convertToUpper);
    ++i;
  }
  if (i == bufSize[0]) {
    bufSize[0] += 2;
    buf = (char*) mem_reallocate (buf, bufSize[0]);
  }
  buf[i] = 0;

  ___CBTPOP;
  return buf;
}

int lineInputReader_t::readNextNonspace (bool convertToUpper) {
  ___CBTPUSH;

  int result = read (convertToUpper);
  if (result == ' ') {
    result = read (convertToUpper);
  }

  ___CBTPOP;
  return result;
}

void lineInputReader_t::rewind (int count) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (count <= 0) {
    errors_printMessageAndExit ("The specified count should be positive");
  } else if (count > m_charsRead) {
    const char* infl1 = count - m_charsRead > 1 ? "s" : "";
    const char* infl2 = m_charsRead != 1 ? "s were" : " was";
    errors_printMessageAndExit ("Cannot rewind %d character%s (only %d character%s read so far)", count, infl1, m_charsRead, infl2);
  }
#endif

  for (int i = 0; i < count; ++i) {
    --m_charsRead;
    --m_position;
    if (isspace (m_buffer[m_position])) {
      --m_position;
      while (isspace (m_buffer[m_position])) {
        --m_position;
      }
      ++m_position;
    }
  }

  ___CBTPOP;
}

void lineInputReader_t::rewindEntirely (void) {
  ___CBTPUSH;

  m_position = m_startPosition;

  ___CBTPOP;
}

void lineInputReader_t::skip (int count) {
  ___CBTPUSH;

  for (int i = 0; i < count; ++i) {
    read (false);
  }

  ___CBTPOP;
}
