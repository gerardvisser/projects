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

#ifndef ___LOMBOK__UTIL__ARRAY_LIST_INCLUDED
#define ___LOMBOK__UTIL__ARRAY_LIST_INCLUDED

#include <lombok/util/list.h>

class arrayList_t : public list_t {
private:
  object_t** _buf;
  int _capacityQuantum;
  int _capacity;
  int _length;

public:
  arrayList_t (int initialCapacity = 16);
  virtual ~arrayList_t (void);

  virtual void            append (object_t* val);
  virtual void            appendarAll (list_t* other);
  virtual void            clear (void);
  virtual bool            contains (object_t* val);
  virtual void            ensureCapacity (int additionalLength);
  virtual void            erase (int index);
  virtual object_t*       get (int index);
  virtual int             indexOf (object_t* val);
  virtual void            insert (int index, object_t* val);
  virtual void            insertar (int index, object_t* val);
  virtual listIterator_t* iterator (void);
  virtual int             length (void);
  virtual object_t*       remove (int index);
  virtual object_t*       removeLast (void);
  virtual void            set (int index, object_t* val);
  virtual void            setar (int index, object_t* val);
  virtual void            setCapacityQuantum (int val);
  virtual void            sort (int (* compare)(object_t*, object_t*));
  virtual void            trimToSize (void);

  REFL_DECL
};

#endif
