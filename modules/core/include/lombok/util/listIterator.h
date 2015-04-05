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

#ifndef ___LOMBOK__UTIL__LIST_ITERATOR_INCLUDED
#define ___LOMBOK__UTIL__LIST_ITERATOR_INCLUDED

#include <lombok/base/class.h>

class listIterator_t : public object_t {
public:
  virtual void      add (object_t* val) = 0;
  virtual void      addar (object_t* val) = 0;
  virtual void      begin (void) = 0;
  virtual void      end (void) = 0;
  virtual void      erase (void) = 0;
  virtual bool      hasNext (void) = 0;
  virtual bool      hasPrevious (void) = 0;
  virtual object_t* next (void) = 0;
  virtual object_t* peek (void) = 0;
  virtual object_t* previous (void) = 0;
  virtual object_t* remove (void) = 0;

  I_REFL_DECL
};

#endif
