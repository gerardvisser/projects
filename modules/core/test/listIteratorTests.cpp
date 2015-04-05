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
#include "listIteratorTests.h"
#include "testObject.h"
#include "listTests.h"

void listIteratorTests::testAdd (list_t* list) {
  ___BTPUSH;
  listIterator_t* iterator = list->iterator ();
  printTestedMethod (iterator);

  testObject_t* obj = new testObject_t (0);
  iterator->add (obj);
  assert (list->length () == 1);
  performRefCountTest (obj, 1);

  iterator->add (new testObject_t (2));
  assert (list->length () == 2);

  iterator->next ();
  iterator->next ();

  iterator->add (new testObject_t (3));
  assert (list->length () == 3);

  iterator->add (new testObject_t (1));
  assert (list->length () == 4);
  iterator->dump ();

  listTests::checkOrderAndNumberOfElements (list, 2, 0, 1, 3, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listIteratorTests::testAddar (list_t* list) {
  ___BTPUSH;
  listIterator_t* iterator = list->iterator ();
  printTestedMethod (iterator);

  testObject_t* obj = new testObject_t (0);
  iterator->addar (obj);
  assert (list->length () == 1);
  performRefCountTest (obj, 2);
  obj->dump ();

  obj = new testObject_t (2);
  iterator->addar (obj);
  assert (list->length () == 2);
  obj->dump ();

  iterator->next ();
  iterator->next ();

  obj = new testObject_t (3);
  iterator->addar (obj);
  assert (list->length () == 3);
  obj->dump ();

  obj = new testObject_t (1);
  iterator->addar (obj);
  assert (list->length () == 4);
  iterator->dump ();
  obj->dump ();

  listTests::checkOrderAndNumberOfElements (list, 2, 0, 1, 3, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listIteratorTests::testBegin (list_t* list) {
  ___BTPUSH;
  list->append (new testObject_t (1));
  list->append (new testObject_t (2));
  listIterator_t* iterator = list->iterator ();
  list->dump ();
  printTestedMethod (iterator);

  iterator->begin ();
  assert (!iterator->hasPrevious ());
  assert (iterator->hasNext ());

  iterator->next ();
  assert (iterator->hasPrevious ());
  assert (iterator->hasNext ());
  iterator->begin ();
  assert (!iterator->hasPrevious ());
  assert (iterator->hasNext ());

  iterator->next ();
  iterator->next ();
  assert (iterator->hasPrevious ());
  assert (!iterator->hasNext ());
  iterator->begin ();
  assert (!iterator->hasPrevious ());
  assert (iterator->hasNext ());

  iterator->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listIteratorTests::testEnd (list_t* list) {
  ___BTPUSH;
  list->append (new testObject_t (1));
  list->append (new testObject_t (2));
  listIterator_t* iterator = list->iterator ();
  list->dump ();
  printTestedMethod (iterator);

  assert (!iterator->hasPrevious ());
  assert (iterator->hasNext ());
  iterator->end ();
  assert (iterator->hasPrevious ());
  assert (!iterator->hasNext ());
  iterator->end ();
  assert (iterator->hasPrevious ());
  assert (!iterator->hasNext ());

  iterator->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listIteratorTests::testErase (list_t* list) {
  ___BTPUSH;
  object_t* seven = new testObject_t (7);
  list->append (new testObject_t (7));
  list->append (new testObject_t (1));
  list->append (new testObject_t (7));
  list->append (new testObject_t (2));
  list->append (new testObject_t (3));
  list->append (new testObject_t (7));
  list->appendar (seven);
  list->append (new testObject_t (4));
  list->append (new testObject_t (5));
  list->append (new testObject_t (7));
  list->append (new testObject_t (6));
  listIterator_t* iterator = list->iterator ();
  printTestedMethod (iterator);

  iterator->end ();
  while (iterator->hasPrevious ()) {
    if (seven->equals (iterator->previous ())) {
      iterator->erase ();
    }
  }
  iterator->dump ();
  seven->dump ();

  assert (list->length () == 6);
  listTests::checkOrderAndNumberOfElements (list, 1, 2, 3, 4, 5, 6, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listIteratorTests::testHasPrevious (list_t* list) {
  ___BTPUSH;
  for (int i = 0; i < 4; ++i) {
    list->append (new testObject_t ());
  }
  listIterator_t* iterator = list->iterator ();
  list->dump ();
  printTestedMethod (iterator);

  assert (!iterator->hasPrevious ());
  iterator->next ();
  assert (iterator->hasPrevious ());
  iterator->next ();
  assert (iterator->hasPrevious ());
  iterator->next ();
  assert (iterator->hasPrevious ());
  iterator->next ();
  assert (iterator->hasPrevious ());

  iterator->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listIteratorTests::testPeek (list_t* list) {
  ___BTPUSH;
  int i;
  for (i = 0; i < 4; ++i) {
    list->append (new testObject_t (i));
  }
  listIterator_t* iterator = list->iterator ();
  list->dump ();
  printTestedMethod (iterator);

  i = 0;
  while (iterator->hasNext ()) {
    assert (((testObject_t*) iterator->peek ())->getValue () == i);
    assert (((testObject_t*) iterator->next ())->getValue () == i);
    ++i;
  }
  assert (i == 4);

  iterator->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listIteratorTests::testPrevious (list_t* list) {
  ___BTPUSH;
  int i;
  for (i = 0; i < 4; ++i) {
    list->append (new testObject_t (i));
  }
  listIterator_t* iterator = list->iterator ();
  list->dump ();
  printTestedMethod (iterator);

  i = 3;
  iterator->end ();
  while (iterator->hasPrevious ()) {
    testObject_t* obj = (testObject_t*) iterator->previous ();
    assert (obj->getValue () == i);
    --i;
  }
  assert (i == -1);

  iterator->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void listIteratorTests::testRemove (list_t* list) {
  ___BTPUSH;
  object_t* seven = new testObject_t (7);
  list->append (new testObject_t (1));
  list->append (new testObject_t (7));
  list->append (new testObject_t (2));
  list->append (new testObject_t (3));
  list->append (new testObject_t (7));
  list->appendar (seven);
  list->append (new testObject_t (4));
  list->append (new testObject_t (5));
  list->append (new testObject_t (7));
  list->append (new testObject_t (6));
  list->append (new testObject_t (7));
  listIterator_t* iterator = list->iterator ();
  printTestedMethod (iterator);

  iterator->end ();
  while (iterator->hasPrevious ()) {
    if (seven->equals (iterator->previous ())) {
      iterator->remove ()->dump ();
    }
  }
  iterator->dump ();
  seven->dump ();

  assert (list->length () == 6);
  listTests::checkOrderAndNumberOfElements (list, 1, 2, 3, 4, 5, 6, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}
