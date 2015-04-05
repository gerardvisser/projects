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

#ifndef ___LOMBOK__UTIL__HASH_MAP_ITERATOR_INCLUDED
#define ___LOMBOK__UTIL__HASH_MAP_ITERATOR_INCLUDED

#include <lombok/util/hashMap.h>

class hashMapIterator_t : public mapIterator_t {
private:
  hashMap_t* const map;
  listIterator_t* bucketIterator;
  long int index;

public:
  hashMapIterator_t (hashMap_t* map);
  virtual ~hashMapIterator_t (void);

  virtual void        begin (void);
  virtual bool        hasNext (void);
  virtual mapEntry_t* next (void);

private:
  void findNextBucketIterator (void);

  REFL_DECL
};

#endif
