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

#ifndef ___MAP_TESTS_INCLUDED
#define ___MAP_TESTS_INCLUDED

#include <lombok/util/map.h>

/* Tests for map_t.
   Implicitly tested methods: map_t::get, map_t::length.
   map_t::iterator will be (implicitly) tested when running the
   mapIteratorTests. */

namespace mapTests {
  /* All map arguments below should be pointers to empty maps.
     The order of the functions shown here, is the order in which
     the tests should be carried out. */

  void testSet (map_t* map);
  void testSetar (map_t* map);
  void testClear (map_t* map);
  void testContains (map_t* map);
  void testErase (map_t* map);
  void testRemove (map_t* map);
}

#endif
