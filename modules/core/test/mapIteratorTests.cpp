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
#include "mapIteratorTests.h"
#include "testObject.h"

static void addTenElements (map_t* map) {
  ___BTPUSH;

  map->set (new testObject_t (1), new testObject_t (1));
  map->set (new testObject_t (2), new testObject_t (4));
  map->set (new testObject_t (3), new testObject_t (9));
  map->set (new testObject_t (4), new testObject_t (16));
  map->set (new testObject_t (5), new testObject_t (25));
  map->set (new testObject_t (6), new testObject_t (36));
  map->set (new testObject_t (7), new testObject_t (49));
  map->set (new testObject_t (8), new testObject_t (64));
  map->set (new testObject_t (9), new testObject_t (81));
  map->set (new testObject_t (10), new testObject_t (100));

  ___BTPOP;
}

void mapIteratorTests::testBegin (map_t* map) {
  ___BTPUSH;
  addTenElements (map);
  mapIterator_t* iterator = map->iterator ();
  printTestedMethod (iterator);

  /* We know from running test mapIteratorTests::testNext that we can call
     iterator->next () ten times without getting back null and that each time
     we get back an entry whose key value is different from the ones we got
     back before. */

  int i, keyValue[10];

  for (i = 0; i < 10; ++i) {
    for (int j = 0; j < i; ++j) {
      assert (((testObject_t*) iterator->next ()->getKey ())->getValue () == keyValue[j]);
    }
    keyValue[i] = ((testObject_t*) iterator->next ()->getKey ())->getValue ();
    iterator->begin ();
  }
  for (i = 0; i < 10; ++i) {
    assert (((testObject_t*) iterator->next ()->getKey ())->getValue () == keyValue[i]);
  }

  iterator->dump ();
  map->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void mapIteratorTests::testHasNext (map_t* map) {
  ___BTPUSH;
  addTenElements (map);
  mapIterator_t* iterator = map->iterator ();
  printTestedMethod (iterator);

  for (int i = 0; i < 10; ++i) {
    assert (iterator->hasNext ());
    iterator->next ();
  }
  assert (!iterator->hasNext ());

  iterator->dump ();
  map->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void mapIteratorTests::testNext (map_t* map) {
  ___BTPUSH;
  addTenElements (map);
  mapIterator_t* iterator = map->iterator ();
  printTestedMethod (iterator);

  arrayList_t* list = new arrayList_t ();
  for (int i = 0; i < 10; ++i) {
    mapEntry_t* entry = iterator->next ();
    assert (entry != NULL);
    assert (!list->contains (entry->getKey ()));
    list->appendar (entry->getKey ());
  }
  assert (iterator->next () == NULL);
  list->dump ();

  iterator->dump ();
  map->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}
