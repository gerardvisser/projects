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
#include <lombok/util/linkedList.h>
#include "linkedListTests.h"
#include "listTests.h"

static void testAppend (void) {
  ___BTPUSH;
  listTests::testAppend (new linkedList_t ());
  ___BTPOP;
}

static void testAppendar (void) {
  ___BTPUSH;
  listTests::testAppendar (new linkedList_t ());
  ___BTPOP;
}

static void testAppendarAll (void) {
  ___BTPUSH;
  listTests::testAppendarAll (new linkedList_t (), new linkedList_t ());
  ___BTPOP;
}

static void testClear (void) {
  ___BTPUSH;
  listTests::testClear (new linkedList_t ());
  ___BTPOP;
}

static void testContains (void) {
  ___BTPUSH;
  listTests::testContains (new linkedList_t ());
  ___BTPOP;
}

static void testEraseLast (void) {
  ___BTPUSH;
  listTests::testEraseLast (new linkedList_t ());
  ___BTPOP;
}

static void testRemoveLast (void) {
  ___BTPUSH;
  listTests::testRemoveLast (new linkedList_t ());
  ___BTPOP;
}

static void testSort (void) {
  ___BTPUSH;
  listTests::testSort (new linkedList_t ());
  ___BTPOP;
}

const struct linkedListTestsStruct linkedListTests = {
  testAppend,
  testAppendar,
  testAppendarAll,
  testClear,
  testContains,
  testEraseLast,
  testRemoveLast,
  testSort
};
