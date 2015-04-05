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

#include <lombok/base/class.h>
#include <lombok/base/errors.h>
#include <lombok/base/hashCode.h>
#include <lombok/base/mem.h>

REFL_IMPL_NO_SUPERCLASS (object_t)

object_t::object_t (void) {
  refCount = 1;
}

object_t::~object_t (void) {
}

void object_t::dump (void) {
  --refCount;
  if (refCount == 0) {
    delete this;
  }
}

bool object_t::equals (object_t* other) {
  return this == other;
}

unsigned int object_t::hashCode (void) {
  return hashCode_get ((long int) this);
}

void object_t::reserve (void) {
  ++refCount;
}

/* Operators: */

void object_t::operator delete (void* p) {
  ___CBTPUSH;

  mem_release (p);

  ___CBTPOP;
}

void object_t::operator delete[] (void* p) {
  ___CBTPUSH;

  mem_release (p);

  ___CBTPOP;
}

void* object_t::operator new (size_t s) {
  ___CBTPUSH;

  void* result = mem_allocate (NULL, s, PTYPE_REF_COUNT_CONTAINER);

  ___CBTPOP;
  return result;
}

void* object_t::operator new[] (size_t s) {
  ___CBTPUSH;

  void* result = mem_allocate (NULL, s, PTYPE_ORDINARY);

  ___CBTPOP;
  return result;
}

/* Debug mode only: */

#ifdef DEBUG_MODE

unsigned long int object_t::getRefCount (void) {
  return refCount;
}

#endif
