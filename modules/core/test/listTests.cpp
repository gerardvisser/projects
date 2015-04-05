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
#include "testObject.h"
#include "listTests.h"
#include <stdarg.h>

static int compare (object_t* testobj1, object_t* testobj2) {
  ___BTPUSH;
  int result = ((testObject_t*) testobj1)->getValue () - ((testObject_t*) testobj2)->getValue ();
  ___BTPOP;
  return result;
}

static int compareReverse (object_t* testobj1, object_t* testobj2) {
  ___BTPUSH;
  int result = -compare (testobj1, testobj2);
  ___BTPOP;
  return result;
}

void listTests::checkOrderAndNumberOfElements (list_t* list, ...) {
  ___BTPUSH;
  va_list argp;
  va_start (argp, list);

  int elemId = va_arg (argp, int);
  listIterator_t* iterator = list->iterator ();
  while (iterator->hasNext () && elemId != -1) {
    assert (((testObject_t*) iterator->next ())->getValue () == elemId);
    elemId = va_arg (argp, int);
  }
  assert (!iterator->hasNext () && elemId == -1);
  iterator->dump ();

  va_end (argp);
  ___BTPOP;
}

void listTests::testAppend (list_t* list) {
  ___BTPUSH;
  printTestedMethod (list);

  for (int i = 0; i < 9; ++i) {
    testObject_t* obj = new testObject_t (i);
    list->append (obj);
    assert (list->length () == i + 1);
    performRefCountTest (obj, 1);
  }
  checkOrderAndNumberOfElements (list, 0, 1, 2, 3, 4, 5, 6, 7, 8, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listTests::testAppendar (list_t* list) {
  ___BTPUSH;
  printTestedMethod (list);

  for (int i = 0; i < 9; ++i) {
    testObject_t* obj = new testObject_t (i);
    list->appendar (obj);
    assert (list->length () == i + 1);
    performRefCountTest (obj, 2);
    obj->dump ();
  }
  checkOrderAndNumberOfElements (list, 0, 1, 2, 3, 4, 5, 6, 7, 8, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listTests::testAppendarAll (list_t* list, list_t* list2) {
  ___BTPUSH;
  printTestedMethod (list);
  assert (list2->length () == 0);

  for (int i = 0; i < 5; ++i) {
    list2->append (new testObject_t (i));
  }
  list->appendarAll (list2);
  checkOrderAndNumberOfElements (list, 0, 1, 2, 3, 4, -1);

  list->dump ();
  list2->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listTests::testClear (list_t* list) {
  ___BTPUSH;
  printTestedMethod (list);

  long allocatedBefore = mem_currentlyAllocatedBytes ();
  list->append (new testObject_t ());
  list->append (new testObject_t ());
  list->append (new testObject_t ());
  list->clear ();
  assert (list->length () == 0);
  performMemoryTest (allocatedBefore);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listTests::testContains (list_t* list) {
  ___BTPUSH;
  printTestedMethod (list);

  testObject_t* obj1 = new testObject_t (3);
  testObject_t* obj2 = new testObject_t (5);
  list->append (new testObject_t (1));
  list->append (new testObject_t (2));
  list->append (new testObject_t (3));
  list->append (new testObject_t (4));
  assert (list->contains (obj1));
  assert (!list->contains (obj2));
  obj1->dump ();
  obj2->dump ();

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listTests::testEraseLast (list_t* list) {
  ___BTPUSH;
  printTestedMethod (list);

  testObject_t* obj = new testObject_t ();
  list->append (new testObject_t ());
  list->append (new testObject_t ());
  list->appendar (obj);

  list->eraseLast ();
  assert (list->length () == 2);
  performRefCountTest (obj, 1);
  obj->dump ();

  list->eraseLast ();
  assert (list->length () == 1);

  list->eraseLast ();
  assert (list->length () == 0);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listTests::testRemoveLast (list_t* list) {
  ___BTPUSH;
  printTestedMethod (list);

  list->append (new testObject_t (1));
  list->append (new testObject_t (2));
  list->append (new testObject_t (3));

  for (int i = 3; i > 0; --i) {
    testObject_t* obj = (testObject_t*) list->removeLast ();
    assert (list->length () == i - 1);
    assert (obj->getValue () == i);
    obj->dump ();
  }

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listTests::testSort (list_t* list) {
  ___BTPUSH;
  printTestedMethod (list);

  list->append (new testObject_t (4));
  list->append (new testObject_t (8));
  list->append (new testObject_t (6));
  list->append (new testObject_t (2));
  list->append (new testObject_t (10));
  list->append (new testObject_t (7));
  list->append (new testObject_t (3));
  list->append (new testObject_t (5));
  list->append (new testObject_t (1));
  list->append (new testObject_t (9));

  list->sort (compare);
  checkOrderAndNumberOfElements (list, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, -1);

  list->sort (compareReverse);
  checkOrderAndNumberOfElements (list, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}
