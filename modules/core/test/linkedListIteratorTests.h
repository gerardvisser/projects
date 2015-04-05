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

#ifndef ___LINKED_LIST_ITERATOR_TESTS_INCLUDED
#define ___LINKED_LIST_ITERATOR_TESTS_INCLUDED

/* Implicitly tested methods:
   `iterator', `length' for linkedList_t and `hasNext', `next' for linkedListIterator_t. */

struct linkedListIteratorTestsStruct {
  void (* testHasPrevious)(void);
  void (* testBegin)(void);
  void (* testEnd)(void);
  void (* testPeek)(void);
  void (* testPrevious)(void);
  void (* testAdd)(void);
  void (* testAddar)(void);
  void (* testErase)(void);
  void (* testRemove)(void);
};

extern const struct linkedListIteratorTestsStruct linkedListIteratorTests;

#endif
