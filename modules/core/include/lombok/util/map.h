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

#ifndef ___LOMBOK__UTIL__MAP_INCLUDED
#define ___LOMBOK__UTIL__MAP_INCLUDED

#include <lombok/util/mapIterator.h>

class map_t : public object_t {
public:
  virtual void           clear (void) = 0;
  virtual bool           contains (object_t* key) = 0;
  virtual void           erase (object_t* key) = 0;
  virtual object_t*      get (object_t* key) = 0;
  virtual mapIterator_t* iterator (void) = 0;
  virtual int            length (void) = 0;
  virtual object_t*      remove (object_t* key) = 0;
  virtual void           set (object_t* key, object_t* value) = 0;
  virtual void           setar (object_t* key, object_t* value) = 0;

  I_REFL_DECL
};

#endif
