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
#include "linkedListNode.h"

#define TYPE_NAME "linkedListNode_t"

#ifdef DEBUG_MODE
static const char* const ___className = TYPE_NAME;
#endif

linkedListNode_t::linkedListNode_t (object_t* value, linkedListNode_t* previous, linkedListNode_t* next) {
  if (previous != NULL) {
    previous->next = this;
  }
  if (next != NULL) {
    next->previous = this;
  }
  this->value = value;
  this->previous = previous;
  this->next = next;
}

linkedListNode_t::~linkedListNode_t (void) {
}

/* Operators: */

INIT_ALLOCATOR (allocator, sizeof (linkedListNode_t), 0x400, PTYPE_INTERNAL, TYPE_NAME)

void* linkedListNode_t::operator new (size_t s) {
  ___CBTPUSH;
  void* result = getAllocator ()->allocate ();
  ___CBTPOP;
  return result;
}

void linkedListNode_t::operator delete (void* p) {
  ___CBTPUSH;
  getAllocator ()->release (p);
  ___CBTPOP;
}
