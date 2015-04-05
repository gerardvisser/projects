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

REFL_IMPL (class_t, object_t)

class_t::class_t (const char* name, size_t size, class_t* superclass) : name (name), size (size), superclass (superclass) {
}

class_t::~class_t (void) {
}

void class_t::dump (void) {
  /* Objects of this type cannot be dynamically created. */
}

bool class_t::isAssignableFrom (class_t* other) {
  ___CBTPUSH;
  bool result;
  if (other == this) {
    result = true;
  } else if (other->superclass == NULL) {
    result = false;
  } else {
    result = isAssignableFrom (other->superclass);
  }
  ___CBTPOP;
  return result;
}

void class_t::reserve (void) {
  /* Objects of this type cannot be dynamically created. */
}

/* Operators: */

void* class_t::operator new (size_t s) {
  /* Objects of this type cannot be dynamically created. */
  //return NULL;
}

void class_t::operator delete (void* p) {
  /* Objects of this type cannot be dynamically created. */
}
