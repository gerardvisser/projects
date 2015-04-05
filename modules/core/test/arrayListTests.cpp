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
#include <lombok/util/arrayList.h>
#include "arrayListTests.h"
#include "listTests.h"
#include "testObject.h"

static void testAppend (void) {
  ___BTPUSH;
  listTests::testAppend (new arrayList_t (4));
  ___BTPOP;
}

static void testAppendar (void) {
  ___BTPUSH;
  listTests::testAppendar (new arrayList_t (4));
  ___BTPOP;
}

static void testAppendarAll (void) {
  ___BTPUSH;
  listTests::testAppendarAll (new arrayList_t (4), new arrayList_t (4));
  ___BTPOP;
}

static void testClear (void) {
  ___BTPUSH;
  listTests::testClear (new arrayList_t (4));
  ___BTPOP;
}

static void testContains (void) {
  ___BTPUSH;
  listTests::testContains (new arrayList_t (4));
  ___BTPOP;
}

static void testEnsureCapacity (void) {
  ___BTPUSH;
  arrayList_t* list = new arrayList_t (4);
  printTestedMethod (list);

  long expectedMinimum = mem_currentlyAllocatedBytes () + 9 * sizeof (void*);
  list->ensureCapacity (4 + 9);
  assert (mem_currentlyAllocatedBytes () >= expectedMinimum);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testErase (void) {
  ___BTPUSH;
  arrayList_t* list = new arrayList_t (4);
  printTestedMethod (list);

  list->append (new testObject_t (1));
  list->append (new testObject_t (2));
  list->append (new testObject_t (3));
  list->append (new testObject_t (4));

  list->erase (2);
  assert (list->length () == 3);
  listTests::checkOrderAndNumberOfElements (list, 1, 2, 4, -1);

  list->erase (0);
  assert (list->length () == 2);
  listTests::checkOrderAndNumberOfElements (list, 2, 4, -1);

  list->erase (1);
  assert (list->length () == 1);
  listTests::checkOrderAndNumberOfElements (list, 2, -1);

  list->erase (0);
  assert (list->length () == 0);
  listTests::checkOrderAndNumberOfElements (list, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testEraseLast (void) {
  ___BTPUSH;
  listTests::testEraseLast (new arrayList_t (4));
  ___BTPOP;
}

static void testGet (void) {
  ___BTPUSH;
  arrayList_t* list = new arrayList_t (4);
  printTestedMethod (list);

  int i;
  for (i = 0; i < 10; ++i) {
    list->append (new testObject_t (i));
  }
  for (i = 9; i > -1; --i) {
    assert (((testObject_t*) list->get (i))->getValue () == i);
  }

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testIndexOf (void) {
  ___BTPUSH;
  arrayList_t* list = new arrayList_t (4);
  printTestedMethod (list);

  list->append (new testObject_t (1));
  list->append (new testObject_t (4));
  list->append (new testObject_t (9));
  list->append (new testObject_t (16));
  list->append (new testObject_t (1));

  testObject_t* obj = new testObject_t (1);
  assert (list->indexOf (obj) == 0);

  list->erase (0);
  assert (list->indexOf (obj) == 3);
  obj->dump ();

  obj = new testObject_t (4);
  assert (list->indexOf (obj) == 0);
  obj->dump ();

  obj = new testObject_t (9);
  assert (list->indexOf (obj) == 1);
  obj->dump ();

  obj = new testObject_t (16);
  assert (list->indexOf (obj) == 2);
  obj->dump ();

  obj = new testObject_t (25);
  assert (list->indexOf (obj) == -1);
  obj->dump ();

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testInsert (void) {
  ___BTPUSH;
  arrayList_t* list = new arrayList_t (4);
  printTestedMethod (list);

  list->append (new testObject_t (2));
  list->append (new testObject_t (4));
  list->append (new testObject_t (6));

  for (int i = 0; i < 4; ++i) {
    testObject_t* obj = new testObject_t (2 * i + 1);
    list->insert (2 * i, obj);
    performRefCountTest (obj, 1);
    assert (list->length () == 4 + i);
  }
  listTests::checkOrderAndNumberOfElements (list, 1, 2, 3, 4, 5, 6, 7, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testInsertar (void) {
  ___BTPUSH;
  arrayList_t* list = new arrayList_t (4);
  printTestedMethod (list);

  list->append (new testObject_t (2));
  list->append (new testObject_t (4));
  list->append (new testObject_t (6));

  for (int i = 0; i < 4; ++i) {
    testObject_t* obj = new testObject_t (2 * i + 1);
    list->insertar (2 * i, obj);
    performRefCountTest (obj, 2);
    assert (list->length () == 4 + i);
    obj->dump ();
  }
  listTests::checkOrderAndNumberOfElements (list, 1, 2, 3, 4, 5, 6, 7, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testRemove (void) {
  ___BTPUSH;
  arrayList_t* list = new arrayList_t (4);
  printTestedMethod (list);

  list->append (new testObject_t (1));
  list->append (new testObject_t (2));
  list->append (new testObject_t (3));
  list->append (new testObject_t (4));

  testObject_t* obj = (testObject_t*) list->remove (2);
  assert (obj->getValue () == 3);
  obj->dump ();
  assert (list->length () == 3);
  listTests::checkOrderAndNumberOfElements (list, 1, 2, 4, -1);

  obj = (testObject_t*) list->remove (0);
  assert (obj->getValue () == 1);
  obj->dump ();
  assert (list->length () == 2);
  listTests::checkOrderAndNumberOfElements (list, 2, 4, -1);

  obj = (testObject_t*) list->remove (1);
  assert (obj->getValue () == 4);
  obj->dump ();
  assert (list->length () == 1);
  listTests::checkOrderAndNumberOfElements (list, 2, -1);

  obj = (testObject_t*) list->remove (0);
  assert (obj->getValue () == 2);
  obj->dump ();
  assert (list->length () == 0);
  listTests::checkOrderAndNumberOfElements (list, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testRemoveLast (void) {
  ___BTPUSH;
  listTests::testRemoveLast (new arrayList_t (4));
  ___BTPOP;
}

static void testSet (void) {
  ___BTPUSH;
  arrayList_t* list = new arrayList_t (4);
  printTestedMethod (list);

  list->append (new testObject_t (1));
  testObject_t* obj = new testObject_t (2);
  list->set (0, obj);
  performRefCountTest (obj, 1);
  listTests::checkOrderAndNumberOfElements (list, 2, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testSetar (void) {
  ___BTPUSH;
  arrayList_t* list = new arrayList_t (4);
  printTestedMethod (list);

  list->append (new testObject_t (1));
  testObject_t* obj = new testObject_t (2);
  list->setar (0, obj);
  performRefCountTest (obj, 2);
  listTests::checkOrderAndNumberOfElements (list, 2, -1);
  obj->dump ();

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testSetCapacityQuantum (void) {
  ___BTPUSH;
  const int initialCapacity = 4;
  arrayList_t* list = new arrayList_t (initialCapacity);
  printTestedMethod (list);

  int i;
  testObject_t* obj = new testObject_t ();
  const long allocatedWithInitialCapacity = mem_currentlyAllocatedBytes ();
  for (i = 0; i < initialCapacity; ++i) {
    list->appendar (obj);
    performMemoryTest (allocatedWithInitialCapacity);
  }
  list->appendar (obj);
  long capQuant = (mem_currentlyAllocatedBytes () - allocatedWithInitialCapacity) / sizeof (void*);
  assert (capQuant > 0);

  list->dump ();
  list = new arrayList_t (initialCapacity);

  capQuant = 3 * capQuant + 1;
  list->setCapacityQuantum (capQuant);
  for (i = 0; i < initialCapacity; ++i) {
    list->appendar (obj);
  }
  list->append (obj);
  performMemoryTest (allocatedWithInitialCapacity + sizeof (void*) * capQuant);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testSort (void) {
  ___BTPUSH;
  listTests::testSort (new arrayList_t (4));
  ___BTPOP;
}

static void testTrimToSize (void) {
  ___BTPUSH;
  arrayList_t* list = new arrayList_t (4);
  printTestedMethod (list);

  list->append (new testObject_t ());
  long expected = mem_currentlyAllocatedBytes () - 3 * sizeof (void*);
  list->trimToSize ();
  performMemoryTest (expected);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

const struct arrayListTestsStruct arrayListTests = {
  testAppend,
  testAppendar,
  testAppendarAll,
  testClear,
  testContains,
  testEnsureCapacity,
  testErase,
  testEraseLast,
  testGet,
  testIndexOf,
  testInsert,
  testInsertar,
  testRemove,
  testRemoveLast,
  testSet,
  testSetar,
  testSetCapacityQuantum,
  testSort,
  testTrimToSize
};
