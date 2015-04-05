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

#ifndef ___LOMBOK__BASE__OBJECT_RELEASE_POOL_INCLUDED
#define ___LOMBOK__BASE__OBJECT_RELEASE_POOL_INCLUDED

#include <lombok/base/allocator.h>
#include <lombok/base/class.h>

class objectReleasePool_t : public object_t {
private:
  object_t* elements;

  objectReleasePool_t (void);
  virtual ~objectReleasePool_t (void);

public:
  void add (allocator_t* obj);
  void add (object_t* obj);

  static objectReleasePool_t* instance (void);

  REFL_DECL

  static void* operator new (size_t s);
};

#endif
