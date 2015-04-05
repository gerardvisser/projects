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

#ifndef ___LOMBOK__UTIL__HASH_MAP_INCLUDED
#define ___LOMBOK__UTIL__HASH_MAP_INCLUDED

#include <lombok/util/map.h>
#include <lombok/util/linkedList.h>

class hashMap_t : public map_t {
private:
  linkedList_t* buckets;
  double loadFactor;
  int capacityQuantum;
  int capacity;
  int rehashLength;
  int _length;

public:
  hashMap_t (int initialCapacity = 16, double loadFactor = 0.75);
  virtual ~hashMap_t (void);

  virtual void           clear (void);
  virtual bool           contains (object_t* key);
  virtual void           erase (object_t* key);
  virtual object_t*      get (object_t* key);
  virtual mapIterator_t* iterator (void);
  virtual int            length (void);
  virtual object_t*      remove (object_t* key);
  virtual void           set (object_t* key, object_t* value);
  virtual void           setar (object_t* key, object_t* value);

private:
  mapEntry_t* getEntry (object_t* key, listIterator_t* bucketIterator);
  void        rehash (void);

  REFL_DECL

  friend class hashMapIterator_t;
};

#endif
