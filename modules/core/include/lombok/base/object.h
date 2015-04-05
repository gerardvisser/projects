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

#ifndef ___LOMBOK__BASE__OBJECT_INCLUDED
#define ___LOMBOK__BASE__OBJECT_INCLUDED

#include <stddef.h>
#include <lombok/base/reflection.h>

class object_t {
private:
  unsigned long int refCount;

public:
  object_t (void);
  virtual ~object_t (void);

  virtual void         dump (void);
  virtual bool         equals (object_t* other);
  virtual unsigned int hashCode (void);
  virtual void         reserve (void);

#ifdef DEBUG_MODE
  virtual unsigned long int getRefCount (void);
#endif

  REFL_DECL

  static void* operator new (size_t s);
  static void operator delete (void* p);
  static void* operator new[] (size_t s);
  static void operator delete[] (void* p);
};

#endif
