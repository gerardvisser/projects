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

#ifndef ___LOMBOK__UTIL__MAP_ENTRY_INCLUDED
#define ___LOMBOK__UTIL__MAP_ENTRY_INCLUDED

#include <lombok/base/class.h>

class mapEntry_t : public object_t {
private:
  object_t* const key;
  object_t* value;

public:
  mapEntry_t (object_t* key, object_t* value);
  virtual ~mapEntry_t (void);

  object_t* getKey (void);
  object_t* getValue (void);

  REFL_DECL

  static void* operator new (size_t s);
  static void operator delete (void* p);

  friend class hashMap_t;
};

#endif
