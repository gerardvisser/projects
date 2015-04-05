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
#include "hashMapIteratorTests.h"
#include "mapIteratorTests.h"

static void testBegin (void) {
  ___BTPUSH;
  mapIteratorTests::testBegin (new hashMap_t (4, 0.5));
  ___BTPOP;
}

static void testHasNext (void) {
  ___BTPUSH;
  mapIteratorTests::testHasNext (new hashMap_t ());
  ___BTPOP;
}

static void testNext (void) {
  ___BTPUSH;
  mapIteratorTests::testNext (new hashMap_t (2, 6));
  ___BTPOP;
}

const struct hashMapIteratorTestsStruct hashMapIteratorTests = {
  testNext,
  testBegin,
  testHasNext
};
