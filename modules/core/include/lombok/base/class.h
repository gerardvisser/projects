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

#ifndef ___LOMBOK__BASE__CLASS_INCLUDED
#define ___LOMBOK__BASE__CLASS_INCLUDED

#include <lombok/base/object.h>

class class_t : public object_t {
public:
  const char* const name;
  const size_t size;
  class_t* const superclass;

  class_t (const char* name, size_t size, class_t* superclass);
  virtual ~class_t (void);

  virtual void dump (void);
  virtual bool isAssignableFrom (class_t* other);
  virtual void reserve (void);

  REFL_DECL

private:
  static void* operator new (size_t s);
  static void operator delete (void* p);
};

#endif
