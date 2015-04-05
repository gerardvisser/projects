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
#include <lombok/base/hashCode.h>
#include "testObject.h"

REFL_IMPL (testObject_t, object_t)

testObject_t::testObject_t (int value) : value (value) {
}

bool testObject_t::equals (object_t* other) {
  ___CBTPUSH;

  bool result = classof (testObject_t)->isAssignableFrom (other->getClass ()) && ((testObject_t*) other)->value == value;

  ___CBTPOP;
  return result;
}

int testObject_t::getValue (void) {
  return value;
}

unsigned int testObject_t::hashCode (void) {
  ___CBTPUSH;

  int result = hashCode_get (value);

  ___CBTPOP;
  return result;
}
