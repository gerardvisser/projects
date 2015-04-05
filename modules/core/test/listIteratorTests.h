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

#ifndef ___LIST_ITERATOR_TESTS_INCLUDED
#define ___LIST_ITERATOR_TESTS_INCLUDED

#include <lombok/util/list.h>

/* Tests for listIterator_t.
   Tests for list_t are expected to have been carried out already.
   Implicitly tested methods:
   `iterator', `length' for list_t and `hasNext', `next' for listIterator_t. */

namespace listIteratorTests {
  /* All list arguments below should be pointers to empty lists.
     The order of the functions shown here, is the order in which
     the tests should be carried out. */

  void testHasPrevious (list_t* list);
  void testBegin (list_t* list);
  void testEnd (list_t* list);
  void testPeek (list_t* list);
  void testPrevious (list_t* list);
  void testAdd (list_t* list);
  void testAddar (list_t* list);
  void testErase (list_t* list);
  void testRemove (list_t* list);
}

#endif
