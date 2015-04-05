/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013 Gerard Visser.

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
#include <string.h>
#include "buffer.h"

#ifdef DEBUG_MODE
static const char* const ___className = "buffer_t";
#endif

buffer_t::buffer_t (int capacity) : cap (capacity) {
  ___CBTPUSH;

  buf = (void**) mem_allocate ("void*[]", capacity * sizeof (void*), PTYPE_INTERNAL);
  len = 0;

  ___CBTPOP;
}

buffer_t::~buffer_t (void) {
  ___CBTPUSH;

  mem_release (buf);

  ___CBTPOP;
}

void buffer_t::add (void* val) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (len == cap) {
    errors_printMessageAndExit("Buffer full");
  }
#endif

  buf[len] = val;
  ++len;

  ___CBTPOP;
}

void* buffer_t::get (int index) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (index < 0 || index >= len) {
    errors_printMessageAndExit ("Index out of bounds");
  }
#endif

  void* result = buf[index];

  ___CBTPOP;
  return result;
}

int buffer_t::length (void) {
  return len;
}

void buffer_t::remove (int index) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (index < 0 || index >= len) {
    errors_printMessageAndExit ("Index out of bounds");
  }
#endif

  --len;
  memmove (buf + index, buf + index + 1, sizeof (void*) * (len - index));

  ___CBTPOP;
}

void* buffer_t::operator new (size_t s) {
  ___CBTPUSH;

  void* result = mem_allocate (___className, s, PTYPE_ORDINARY);

  ___CBTPOP;
  return result;
}

void buffer_t::operator delete (void* p) {
  ___CBTPUSH;

  mem_release (p);

  ___CBTPOP;
}
