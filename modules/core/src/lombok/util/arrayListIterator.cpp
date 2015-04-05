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

#include <lombok/base/allocator.h>
#include <lombok/base/errors.h>
#include "arrayListIterator.h"

REFL_IMPL (arrayListIterator_t, listIterator_t)

arrayListIterator_t::arrayListIterator_t (arrayList_t* list) : list (list), index (0) {
  ___CBTPUSH;
  list->reserve ();
  ___CBTPOP;
}

arrayListIterator_t::~arrayListIterator_t (void) {
  ___CBTPUSH;
  list->dump ();
  ___CBTPOP;
}

void arrayListIterator_t::add (object_t* val) {
  ___CBTPUSH;
  list->insert (index, val);
  ___CBTPOP;
}

void arrayListIterator_t::addar (object_t* val) {
  ___CBTPUSH;
  list->insertar (index, val);
  ___CBTPOP;
}

void arrayListIterator_t::begin (void) {
  ___CBTPUSH;
  index = 0;
  ___CBTPOP;
}

void arrayListIterator_t::end (void) {
  ___CBTPUSH;
  index = list->length ();
  ___CBTPOP;
}

void arrayListIterator_t::erase (void) {
  ___CBTPUSH;
  list->erase (index);
  ___CBTPOP;
}

bool arrayListIterator_t::hasNext (void) {
  ___CBTPUSH;
  bool result = index < list->length ();
  ___CBTPOP;
  return result;
}

bool arrayListIterator_t::hasPrevious (void) {
  ___CBTPUSH;
  bool result = index > 0;
  ___CBTPOP;
  return result;
}

object_t* arrayListIterator_t::next (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (!hasNext ()) {
    errors_printMessageAndExit ("There is no next element");
  }
#endif
  object_t* result = list->get (index);
  ++index;
  ___CBTPOP;
  return result;
}

object_t* arrayListIterator_t::peek (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (!hasNext ()) {
    errors_printMessageAndExit ("There is no next element");
  }
#endif
  object_t* result = list->get (index);
  ___CBTPOP;
  return result;
}

object_t* arrayListIterator_t::previous (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (!hasPrevious ()) {
    errors_printMessageAndExit ("There is no previous element");
  }
#endif
  --index;
  object_t* result = list->get (index);
  ___CBTPOP;
  return result;
}

object_t* arrayListIterator_t::remove (void) {
  ___CBTPUSH;
  object_t* result = list->remove (index);
  ___CBTPOP;
  return result;
}

/* Operators: */

INIT_ALLOCATOR (allocator, sizeof (arrayListIterator_t), 16)

void* arrayListIterator_t::operator new (size_t s) {
  ___CBTPUSH;
  void* result = getAllocator ()->allocate ();
  ___CBTPOP;
  return result;
}

void arrayListIterator_t::operator delete (void* p) {
  ___CBTPUSH;
  getAllocator ()->release (p);
  ___CBTPOP;
}
