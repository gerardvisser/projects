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
#include "linkedListIterator.h"

REFL_IMPL (linkedListIterator_t, listIterator_t)

linkedListIterator_t::linkedListIterator_t (linkedList_t* list) : list (list) {
  ___CBTPUSH;
  list->reserve ();
  _next = (linkedListNode_t*) list->first;
  ___CBTPOP;
}

linkedListIterator_t::~linkedListIterator_t (void) {
  ___CBTPUSH;
  list->dump ();
  ___CBTPOP;
}

void linkedListIterator_t::add (object_t* val) {
  ___CBTPUSH;
  if (_next != NULL) {
    _next = new linkedListNode_t (val, _next->previous, _next);
    if (_next->previous == NULL) {
      list->first = _next;
    }
  } else if (list->last != NULL) {
    _next = new linkedListNode_t (val, (linkedListNode_t*) list->last, NULL);
    list->last = _next;
  } else {
    _next = new linkedListNode_t (val, NULL, NULL);
    list->first = _next;
    list->last = _next;
  }
  ++list->_length;
  ___CBTPOP;
}

void linkedListIterator_t::addar (object_t* val) {
  ___CBTPUSH;
  add (val);
  val->reserve ();
  ___CBTPOP;
}

void linkedListIterator_t::begin (void) {
  ___CBTPUSH;
  _next = (linkedListNode_t*) list->first;
  ___CBTPOP;
}

void linkedListIterator_t::end (void) {
  ___CBTPUSH;
  _next = NULL;
  ___CBTPOP;
}

void linkedListIterator_t::erase (void) {
  ___CBTPUSH;
  remove ()->dump ();
  ___CBTPOP;
}

bool linkedListIterator_t::hasNext (void) {
  ___CBTPUSH;
  bool result = _next != NULL;
  ___CBTPOP;
  return result;
}

bool linkedListIterator_t::hasPrevious (void) {
  ___CBTPUSH;
  bool result;
  if (_next != NULL) {
    result = _next->previous != NULL;
  } else {
    result = list->last != NULL;
  }
  ___CBTPOP;
  return result;
}

object_t* linkedListIterator_t::next (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (!hasNext ()) {
    errors_printMessageAndExit ("There is no next element");
  }
#endif
  object_t* result = _next->value;
  _next = _next->next;
  ___CBTPOP;
  return result;
}

object_t* linkedListIterator_t::peek (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (!hasNext ()) {
    errors_printMessageAndExit ("There is no next element");
  }
#endif
  object_t* result = _next->value;
  ___CBTPOP;
  return result;
}

object_t* linkedListIterator_t::previous (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (!hasPrevious ()) {
    errors_printMessageAndExit ("There is no previous element");
  }
#endif
  if (_next != NULL) {
    _next = _next->previous;
  } else {
    _next = (linkedListNode_t*) list->last;
  }
  object_t* result = _next->value;
  ___CBTPOP;
  return result;
}

object_t* linkedListIterator_t::remove (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (!hasNext ()) {
    errors_printMessageAndExit ("Already at the end of the list");
  }
#endif
  --list->_length;
  linkedListNode_t* node = _next;
  object_t* result = _next->value;
  if (node->previous != NULL) {
    if (node->next != NULL) {
      node->next->previous = node->previous;
      node->previous->next = node->next;
      _next = node->next;
    } else {
      list->last = node->previous;
      node->previous->next = NULL;
      _next = NULL;
    }
  } else if (node->next != NULL) {
    list->first = node->next;
    node->next->previous = NULL;
    _next = node->next;
  } else {
    list->first = NULL;
    list->last = NULL;
    _next = NULL;
  }
  delete node;
  ___CBTPOP;
  return result;
}

/* Operators: */

INIT_ALLOCATOR (allocator, sizeof (linkedListIterator_t), 16)

void* linkedListIterator_t::operator new (size_t s) {
  ___CBTPUSH;
  void* result = getAllocator ()->allocate ();
  ___CBTPOP;
  return result;
}

void linkedListIterator_t::operator delete (void* p) {
  ___CBTPUSH;
  getAllocator ()->release (p);
  ___CBTPOP;
}
