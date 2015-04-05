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
#include "charBuf.h"

#ifdef DEBUG_MODE
static const char* const ___className = "charBuf_t";
#endif

charBuf_t::charBuf_t (int minSize, char* array, int size) {
  ___CBTPUSH;

  this->allocated = size < minSize;
  if (this->allocated) {
    size = minSize;
    array = (char*) mem_allocate ("char[]", size, PTYPE_ORDINARY);
  }
  this->delta = 0.2 * size;
  if (this->delta < 8) {
    this->delta = 8;
  }
  this->buf = array;
  this->size = size;
  this->index = 0;

  ___CBTPOP;
}

charBuf_t::~charBuf_t (void) {
  /* buf will generally be used elsewhere: not to be freed. */
}

void charBuf_t::append (char cr) {
  ___CBTPUSH;

  if (index == size) {
    enlarge ();
  }
  buf[index] = cr;
  ++index;

  ___CBTPOP;
}

int charBuf_t::bytesAppended (void) {
  return index;
}

void charBuf_t::enlarge (void) {
  ___CBTPUSH;

  size += delta;
  if (allocated) {
    buf = (char*) mem_reallocate (buf, size);
  } else {
    char* oldbuf = buf;
    buf = (char*) mem_allocate ("char[]", size, PTYPE_ORDINARY);
    memcpy (buf, oldbuf, index);
    allocated = true;
  }

  ___CBTPOP;
}

char* charBuf_t::get (void) {
  return buf;
}
