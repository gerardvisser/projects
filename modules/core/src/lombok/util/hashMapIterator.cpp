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

#include <lombok/base/errors.h>
#include "hashMapIterator.h"

REFL_IMPL (hashMapIterator_t, mapIterator_t)

hashMapIterator_t::hashMapIterator_t (hashMap_t* map) : map (map) {
  ___CBTPUSH;

  map->reserve (); /* TODO: weghalen? */
  index = 0;
  bucketIterator = NULL;
  findNextBucketIterator ();

  ___CBTPOP;
}

hashMapIterator_t::~hashMapIterator_t (void) {
  ___CBTPUSH;

  if (bucketIterator != NULL) {
    bucketIterator->dump ();
  }
  map->dump (); /* TODO: weghalen? */

  ___CBTPOP;
}

void hashMapIterator_t::begin (void) {
  ___CBTPUSH;

  index = 0;
  findNextBucketIterator ();

  ___CBTPOP;
}

void hashMapIterator_t::findNextBucketIterator (void) {
  ___CBTPUSH;

  if (bucketIterator != NULL) {
    bucketIterator->dump ();
  }
  while (index < map->capacity && map->buckets[index].length () == 0) {
    ++index;
  }
  if (index < map->capacity) {
    bucketIterator = map->buckets[index].iterator ();
    ++index;
  } else {
    bucketIterator = NULL;
  }

  ___CBTPOP;
}

bool hashMapIterator_t::hasNext (void) {
  ___CBTPUSH;

  bool result = bucketIterator != NULL;
  if (result && !bucketIterator->hasNext ()) {
    findNextBucketIterator ();
    result = bucketIterator != NULL;
  }

  ___CBTPOP;
  return result;
}

mapEntry_t* hashMapIterator_t::next (void) {
  ___CBTPUSH;

  mapEntry_t* result = hasNext () ? (mapEntry_t*) bucketIterator->next () : NULL;

  ___CBTPOP;
  return result;
}
