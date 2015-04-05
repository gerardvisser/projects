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

#include <lombok/testing/testUtils.h>
#include "objectTests.h"

static void testAllMethods (void) {
  ___BTPUSH;
  printTest ("All methods of object_t");

  object_t* obj = new object_t ();

  performRefCountTest (obj, 1);
  obj->reserve ();
  performRefCountTest (obj, 2);
  obj->dump ();
  performRefCountTest (obj, 1);

  assert (obj->equals (obj));
  object_t* obj2 = new object_t ();
  assert (!obj->equals (obj2));
  obj2->dump ();

  assert (obj->getClass () == classof (object_t));

  obj->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

const struct objectTestsStruct objectTests = {
  testAllMethods
};
