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

#ifndef ___LOMBOK__UTIL__LINKED_LIST_INCLUDED
#define ___LOMBOK__UTIL__LINKED_LIST_INCLUDED

#include <lombok/util/list.h>

class linkedList_t : public list_t {
private:
  void* first;
  void* last;
  int _length;

public:
  linkedList_t (void);
  virtual ~linkedList_t (void);

  virtual void            append (object_t* val);
  virtual void            appendarAll (list_t* other);
  virtual void            clear (void);
  virtual bool            contains (object_t* val);
  virtual listIterator_t* iterator (void);
  virtual int             length (void);
  virtual object_t*       removeLast (void);
  virtual void            sort (int (* compare)(object_t*, object_t*));

  REFL_DECL

  friend class linkedListIterator_t;
};

#endif
