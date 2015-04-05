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

#ifndef ___HASH_MAP_TESTS_INCLUDED
#define ___HASH_MAP_TESTS_INCLUDED

/* Implicitly tested methods: hashMap_t::get, hashMap_t::length.
   hashMap_t::iterator will be (implicitly) tested when running the
   hashMapIteratorTests. */

struct hashMapTestsStruct {
  void (* testSet)(void);
  void (* testSetar)(void);
  void (* testClear)(void);
  void (* testContains)(void);
  void (* testErase)(void);
  void (* testRemove)(void);
};

extern const struct hashMapTestsStruct hashMapTests;

#endif
