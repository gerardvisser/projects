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

#ifndef ___LOMBOK__UTIL__ARRAY_LIST_ITERATOR_INCLUDED
#define ___LOMBOK__UTIL__ARRAY_LIST_ITERATOR_INCLUDED

#include <lombok/util/arrayList.h>

class arrayListIterator_t : public listIterator_t {
private:
  arrayList_t* const list;
  long int index;

public:
  arrayListIterator_t (arrayList_t* list);
  virtual ~arrayListIterator_t (void);

  virtual void      add (object_t* val);
  virtual void      addar (object_t* val);
  virtual void      begin (void);
  virtual void      end (void);
  virtual void      erase (void);
  virtual bool      hasNext (void);
  virtual bool      hasPrevious (void);
  virtual object_t* next (void);
  virtual object_t* peek (void);
  virtual object_t* previous (void);
  virtual object_t* remove (void);

  REFL_DECL

  static void* operator new (size_t s);
  static void operator delete (void* p);
};

#endif
