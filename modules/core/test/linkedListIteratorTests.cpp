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
#include "linkedListIteratorTests.h"
#include <lombok/util/linkedList.h>
#include "listIteratorTests.h"

static void testAdd (void) {
  ___BTPUSH;
  listIteratorTests::testAdd (new linkedList_t ());
  ___BTPOP;
}

static void testAddar (void) {
  ___BTPUSH;
  listIteratorTests::testAddar (new linkedList_t ());
  ___BTPOP;
}

static void testBegin (void) {
  ___BTPUSH;
  listIteratorTests::testBegin (new linkedList_t ());
  ___BTPOP;
}

static void testEnd (void) {
  ___BTPUSH;
  listIteratorTests::testEnd (new linkedList_t ());
  ___BTPOP;
}

static void testErase (void) {
  ___BTPUSH;
  listIteratorTests::testErase (new linkedList_t ());
  ___BTPOP;
}

static void testHasPrevious (void) {
  ___BTPUSH;
  listIteratorTests::testHasPrevious (new linkedList_t ());
  ___BTPOP;
}

static void testPeek (void) {
  ___BTPUSH;
  listIteratorTests::testPeek (new linkedList_t ());
  ___BTPOP;
}

static void testPrevious (void) {
  ___BTPUSH;
  listIteratorTests::testPrevious (new linkedList_t ());
  ___BTPOP;
}

static void testRemove (void) {
  ___BTPUSH;
  listIteratorTests::testRemove (new linkedList_t ());
  ___BTPOP;
}

const struct linkedListIteratorTestsStruct linkedListIteratorTests = {
  testHasPrevious,
  testBegin,
  testEnd,
  testPeek,
  testPrevious,
  testAdd,
  testAddar,
  testErase,
  testRemove
};
