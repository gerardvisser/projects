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
#include <lombok/util/hashMap.h>
#include "hashMapTests.h"
#include "mapTests.h"

static void testClear (void) {
  ___BTPUSH;
  mapTests::testClear (new hashMap_t (3, 0.1));
  ___BTPOP;
}

static void testContains (void) {
  ___BTPUSH;
  mapTests::testContains (new hashMap_t (3, 2));
  ___BTPOP;
}

static void testErase (void) {
  ___BTPUSH;
  mapTests::testErase (new hashMap_t ());
  ___BTPOP;
}

static void testRemove (void) {
  ___BTPUSH;
  mapTests::testRemove (new hashMap_t (4));
  ___BTPOP;
}

static void testSet (void) {
  ___BTPUSH;
  mapTests::testSet (new hashMap_t (4, 0.5));
  ___BTPOP;
}

static void testSetar (void) {
  ___BTPUSH;
  mapTests::testSetar (new hashMap_t (0, -10));
  ___BTPOP;
}

const struct hashMapTestsStruct hashMapTests = {
  testSet,
  testSetar,
  testClear,
  testContains,
  testErase,
  testRemove
};
