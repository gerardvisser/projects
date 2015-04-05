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
#include "mapTests.h"

#define KEY_1   1
#define KEY_2   2
#define KEY_3   3
#define KEY_4   4
#define KEY_5   5
#define KEY_6   6
#define VALUE_1 7
#define VALUE_2 8
#define VALUE_3 9
#define VALUE_4 10
#define VALUE_5 KEY_5
#define VALUE_6 11

static void addFiveElements (map_t* map) {
  ___BTPUSH;

  map->set (new testObject_t (KEY_1), new testObject_t (VALUE_1));
  assert (map->length () == 1);
  map->set (new testObject_t (KEY_2), new testObject_t (VALUE_2));
  assert (map->length () == 2);
  map->set (new testObject_t (KEY_3), new testObject_t (VALUE_3));
  assert (map->length () == 3);
  map->set (new testObject_t (KEY_4), new testObject_t (VALUE_4));
  assert (map->length () == 4);
  testObject_t* kv = new testObject_t (KEY_5);
  kv->reserve ();
  map->set (kv, kv);
  assert (map->length () == 5);

  ___BTPOP;
}

void mapTests::testClear (map_t* map) {
  ___BTPUSH;
  printTestedMethod (map);

  testObject_t key1 (KEY_1);
  testObject_t key2 (KEY_2);
  testObject_t key3 (KEY_3);
  testObject_t key4 (KEY_4);
  testObject_t key5 (KEY_5);
  addFiveElements (map);
  map->clear ();
  assert (map->length () == 0);
  assert (map->get (&key1) == NULL);
  assert (map->get (&key2) == NULL);
  assert (map->get (&key3) == NULL);
  assert (map->get (&key4) == NULL);
  assert (map->get (&key5) == NULL);

  map->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void mapTests::testContains (map_t* map) {
  ___BTPUSH;
  printTestedMethod (map);

  testObject_t key1 (KEY_1);
  testObject_t key2 (KEY_2);
  testObject_t key3 (KEY_3);
  testObject_t key4 (KEY_4);
  testObject_t key5 (KEY_5);
  testObject_t key6 (KEY_6);
  addFiveElements (map);
  assert (map->contains (&key1));
  assert (map->contains (&key2));
  assert (map->contains (&key3));
  assert (map->contains (&key4));
  assert (map->contains (&key5));
  assert (!map->contains (&key6));

  map->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void mapTests::testErase (map_t* map) {
  ___BTPUSH;
  printTestedMethod (map);

  testObject_t key (KEY_1);
  testObject_t value (VALUE_1);
  map->setar (&key, &value);
  assert (map->length () == 1);
  performRefCountTest (&key, 2);
  performRefCountTest (&value, 2);

  map->erase (&key);
  assert (map->length () == 0);
  performRefCountTest (&key, 1);
  performRefCountTest (&value, 1);

  map->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void mapTests::testRemove (map_t* map) {
  ___BTPUSH;
  printTestedMethod (map);

  addFiveElements (map);
  testObject_t key (KEY_6);
  testObject_t value (VALUE_6);
  map->set (&key, &value);
  key.reserve ();
  assert (map->length () == 6);
  performRefCountTest (&key, 2);
  performRefCountTest (&value, 1);

  testObject_t* returnedValue = (testObject_t*) map->remove (&key);
  assert (map->length () == 5);
  performRefCountTest (&key, 1);
  performRefCountTest (&value, 1);
  assert (returnedValue == &value);

  map->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void mapTests::testSet (map_t* map) {
  ___BTPUSH;
  printTestedMethod (map);

  testObject_t* key = new testObject_t (KEY_1);
  testObject_t* altkey = new testObject_t (KEY_1);
  testObject_t* value = new testObject_t (VALUE_1);
  testObject_t* altvalue = new testObject_t (VALUE_6);
  altvalue->reserve ();
  altkey->reserve ();

  assert (map->length () == 0);
  assert (map->get (key) == NULL);

  map->set (key, altvalue);
  assert (map->length () == 1);
  performRefCountTest (key, 1);
  performRefCountTest (value, 1);
  performRefCountTest (altkey, 2);
  performRefCountTest (altvalue, 2);
  assert (((testObject_t*) map->get (key))->getValue () == VALUE_6);

  map->set (altkey, value);
  assert (map->length () == 1);
  performRefCountTest (key, 1);
  performRefCountTest (value, 1);
  performRefCountTest (altkey, 1);
  performRefCountTest (altvalue, 1);
  assert (((testObject_t*) map->get (key))->getValue () == VALUE_1);
  altvalue->dump ();
  altkey->dump ();

  addFiveElements (map);

  key = new testObject_t (KEY_1);
  assert (((testObject_t*) map->get (key))->getValue () == VALUE_1);
  key->dump ();
  key = new testObject_t (KEY_2);
  assert (((testObject_t*) map->get (key))->getValue () == VALUE_2);
  key->dump ();
  key = new testObject_t (KEY_3);
  assert (((testObject_t*) map->get (key))->getValue () == VALUE_3);
  key->dump ();
  key = new testObject_t (KEY_4);
  assert (((testObject_t*) map->get (key))->getValue () == VALUE_4);
  key->dump ();
  key = new testObject_t (KEY_5);
  assert (((testObject_t*) map->get (key))->getValue () == VALUE_5);
  key->dump ();
  key = new testObject_t (KEY_6);
  assert (map->get (key) == NULL);
  key->dump ();

  map->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void mapTests::testSetar (map_t* map) {
  ___BTPUSH;
  printTestedMethod (map);

  addFiveElements (map);

  testObject_t* key = new testObject_t (KEY_6);
  testObject_t* altkey = new testObject_t (KEY_6);
  testObject_t* value = new testObject_t (VALUE_6);
  testObject_t* altvalue = new testObject_t (VALUE_1);

  map->setar (key, altvalue);
  assert (map->length () == 6);
  performRefCountTest (key, 2);
  performRefCountTest (value, 1);
  performRefCountTest (altkey, 1);
  performRefCountTest (altvalue, 2);
  assert (((testObject_t*) map->get (key))->getValue () == VALUE_1);

  map->setar (altkey, value);
  assert (map->length () == 6);
  performRefCountTest (key, 2);
  performRefCountTest (value, 2);
  performRefCountTest (altkey, 1);
  performRefCountTest (altvalue, 1);
  assert (((testObject_t*) map->get (key))->getValue () == VALUE_6);
  altvalue->dump ();
  altkey->dump ();
  value->dump ();
  key->dump ();

  map->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}
