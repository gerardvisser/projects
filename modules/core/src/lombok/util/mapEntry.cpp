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

#include <lombok/base/allocator.h>
#include <lombok/base/errors.h>
#include <lombok/util/mapEntry.h>

REFL_IMPL (mapEntry_t, object_t)

mapEntry_t::mapEntry_t (object_t* key, object_t* value) : key (key), value (value) {
}

mapEntry_t::~mapEntry_t (void) {
  ___CBTPUSH;

  key->dump ();
  value->dump ();

  ___CBTPOP;
}

object_t* mapEntry_t::getKey (void) {
  return key;
}

object_t* mapEntry_t::getValue (void) {
  return value;
}

/* Operators: */

INIT_ALLOCATOR (allocator, sizeof (mapEntry_t), 0x400)

void* mapEntry_t::operator new (size_t s) {
  ___CBTPUSH;

  void* result = getAllocator ()->allocate ();

  ___CBTPOP;
  return result;
}

void mapEntry_t::operator delete (void* p) {
  ___CBTPUSH;

  getAllocator ()->release (p);

  ___CBTPOP;
}
