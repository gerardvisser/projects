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

#ifndef ___LIST_TESTS_INCLUDED
#define ___LIST_TESTS_INCLUDED

#include <lombok/util/list.h>

/* Tests for list_t.
   Implicitly tested methods:
   `iterator', `length' for list_t and `hasNext', `next' for listIterator_t. */

namespace listTests {
  /* list is expected to be a list of testObject_t*.
     ... is a -1 terminated sequence of ints. */
  void checkOrderAndNumberOfElements (list_t* list, ...);

  /* All list arguments below should be pointers to empty lists. */
  void testAppend (list_t* list);
  void testAppendar (list_t* list);
  void testAppendarAll (list_t* list, list_t* list2);
  void testClear (list_t* list);
  void testContains (list_t* list);
  void testEraseLast (list_t* list);
  void testRemoveLast (list_t* list);
  void testSort (list_t* list);
}

#endif
