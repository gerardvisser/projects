/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2012, 2013 Gerard Visser.

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
#include <lombok/util/hashMap.h>
#include "hashMapIterator.h"
#include "linkedListIterator.h"

REFL_IMPL (hashMap_t, map_t)

hashMap_t::hashMap_t (int initialCapacity, double loadFactor) {
  ___CBTPUSH;

  if (initialCapacity < 2) {
    initialCapacity = 16;
  }
  if (loadFactor <= 1.0/initialCapacity) {
    loadFactor = 0.75;
  }
  this->buckets = new linkedList_t[initialCapacity];
  this->rehashLength = (int)(loadFactor * initialCapacity);
  this->capacityQuantum = initialCapacity;
  this->capacity = initialCapacity;
  this->loadFactor = loadFactor;
  this->_length = 0;

  ___CBTPOP;
}

hashMap_t::~hashMap_t (void) {
  ___CBTPUSH;

  delete[] buckets;

  ___CBTPOP;
}

void hashMap_t::clear (void) {
  ___CBTPUSH;

  for (int i = 0; i < capacity; ++i) {
    buckets[i].clear ();
  }
  _length = 0;

  ___CBTPOP;
}

bool hashMap_t::contains (object_t* key) {
  ___CBTPUSH;

  linkedListIterator_t bucketIterator (buckets + key->hashCode () % capacity);
  bool result = getEntry (key, &bucketIterator) != NULL;

  ___CBTPOP;
  return result;
}

void hashMap_t::erase (object_t* key) {
  ___CBTPUSH;

  linkedListIterator_t bucketIterator (buckets + key->hashCode () % capacity);
  if (getEntry (key, &bucketIterator) != NULL) {
    bucketIterator.erase ();
    --_length;
  }

  ___CBTPOP;
}

object_t* hashMap_t::get (object_t* key) {
  ___CBTPUSH;

  linkedListIterator_t bucketIterator (buckets + key->hashCode () % capacity);
  mapEntry_t* entry = getEntry (key, &bucketIterator);

  ___CBTPOP;
  return entry != NULL ? entry->value : NULL;
}

mapEntry_t* hashMap_t::getEntry (object_t* key, listIterator_t* bucketIterator) {
  ___CBTPUSH;

  mapEntry_t* entry;
  if (bucketIterator->hasNext ()) {

    bool entryFound;
    bucketIterator->end ();
    do {
      entry = (mapEntry_t*) bucketIterator->previous ();
      entryFound = key->equals (entry->key);
    } while (!entryFound && bucketIterator->hasPrevious ());

    if (!entryFound) {
      entry = NULL;
    }

  } else {
    entry = NULL;
  }

  ___CBTPOP;
  return entry;
}

mapIterator_t* hashMap_t::iterator (void) {
  ___CBTPUSH;

  mapIterator_t* result = new hashMapIterator_t (this);

  ___CBTPOP;
  return result;
}

int hashMap_t::length (void) {
  return _length;
}

void hashMap_t::rehash (void) {
  ___CBTPUSH;

  int oldCapacity = capacity;
  linkedList_t* oldBuckets = buckets;
  capacity += capacityQuantum;
  buckets = new linkedList_t[capacity];
  rehashLength = (int)(loadFactor * capacity);

  for (int i = 0; i < oldCapacity; ++i) {
    listIterator_t* bucketIterator = oldBuckets[i].iterator ();
    while (bucketIterator->hasNext ()) {
      mapEntry_t* entry = (mapEntry_t*) bucketIterator->next ();
      buckets[entry->key->hashCode () % capacity].appendar (entry);
    }
    bucketIterator->dump ();
  }
  delete[] oldBuckets;

  ___CBTPOP;
}

object_t* hashMap_t::remove (object_t* key) {
  ___CBTPUSH;

  object_t* result;
  linkedListIterator_t bucketIterator (buckets + key->hashCode () % capacity);
  if (getEntry (key, &bucketIterator) != NULL) {
    mapEntry_t* entry = (mapEntry_t*) bucketIterator.remove ();
    result = entry->value;
    result->reserve ();
    entry->dump ();
    --_length;
  } else {
    result = NULL;
  }

  ___CBTPOP;
  return result;
}

void hashMap_t::set (object_t* key, object_t* value) {
  ___CBTPUSH;

  const int keyHash = key->hashCode ();
  listIterator_t* bucketIterator = buckets[keyHash % capacity].iterator ();
  mapEntry_t* entry = getEntry (key, bucketIterator);
  bucketIterator->dump ();
  if (entry == NULL) {
    if (_length == rehashLength) {
      rehash ();
    }
    buckets[keyHash % capacity].append (new mapEntry_t (key, value));
    ++_length;
  } else {
    entry->value->dump ();
    entry->value = value;
    key->dump ();
  }

  ___CBTPOP;
}

void hashMap_t::setar (object_t* key, object_t* value) {
  ___CBTPUSH;

  const int keyHash = key->hashCode ();
  listIterator_t* bucketIterator = buckets[keyHash % capacity].iterator ();
  mapEntry_t* entry = getEntry (key, bucketIterator);
  bucketIterator->dump ();
  value->reserve ();
  if (entry == NULL) {
    if (_length == rehashLength) {
      rehash ();
    }
    buckets[keyHash % capacity].append (new mapEntry_t (key, value));
    key->reserve ();
    ++_length;
  } else {
    entry->value->dump ();
    entry->value = value;
  }

  ___CBTPOP;
}
