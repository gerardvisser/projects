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

#include <lombok/base/errors.h>
#include <lombok/util/linkedList.h>
#include "linkedListIterator.h"

REFL_IMPL (linkedList_t, list_t)

#define node_t linkedListNode_t

linkedList_t::linkedList_t (void) {
  ___CBTPUSH;

  first = NULL;
  last = NULL;
  _length = 0;

  ___CBTPOP;
}

linkedList_t::~linkedList_t (void) {
  ___CBTPUSH;

  clear ();

  ___CBTPOP;
}

void linkedList_t::append (object_t* val) {
  ___CBTPUSH;

  if (last != NULL) {
    last = new node_t (val, (node_t*) last, NULL);
  } else {
    last = new node_t (val, NULL, NULL);
    first = last;
  }
  ++_length;

  ___CBTPOP;
}

void linkedList_t::appendarAll (list_t* other) {
  ___CBTPUSH;

  listIterator_t* iter = other->iterator ();
  if (iter->hasNext ()) {
    appendar (iter->next ());
    while (iter->hasNext ()) {
      object_t* obj = iter->next ();
      last = new node_t (obj, (node_t*) last, NULL);
      obj->reserve ();
      ++_length;
    }
  }
  iter->dump ();

  ___CBTPOP;
}

void linkedList_t::clear (void) {
  ___CBTPUSH;

  node_t* node = (node_t*) first;
  while (node != NULL) {
    node_t* next = node->next;
    node->value->dump ();
    delete node;
    node = next;
  }

  first = NULL;
  last = NULL;
  _length = 0;

  ___CBTPOP;
}

bool linkedList_t::contains (object_t* val) {
  ___CBTPUSH;

  node_t* node = (node_t*) first;
  while (!(node == NULL || val->equals (node->value))) {
    node = node->next;
  }

  ___CBTPOP;
  return node != NULL;
}

listIterator_t* linkedList_t::iterator (void) {
  ___CBTPUSH;

  listIterator_t* result = new linkedListIterator_t (this);

  ___CBTPOP;
  return result;
}

int linkedList_t::length (void) {
  return _length;
}

object_t* linkedList_t::removeLast (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (_length == 0) {
    errors_printMessageAndExit ("List is empty");
  }
#endif

  --_length;
  node_t* node = (node_t*) last;
  last = node->previous;
  if (last != NULL) {
    node->previous->next = NULL;
  } else {
    first = NULL;
  }
  object_t* result = node->value;
  delete node;

  ___CBTPOP;
  return result;
}

void linkedList_t::sort (int (* compare)(object_t*, object_t*)) {
  ___CBTPUSH;

  if (_length > 1) {
    /*
       a   y           b   x   c
       .   .   .   .   .   .   .
         ^                 |
         |_________________|
    */
    node_t* x = ((node_t*) first)->next;
    while (x != NULL) {
      node_t* next = x->next;
      object_t* obj = x->value;
      node_t* y = (node_t*) first;
      while (y != x  &&  compare (obj, y->value) > 0) {
        y = y->next;
      }
      if (y != x) {
        node_t* a = y->previous;
        node_t* b = x->previous;
        node_t* c = x->next;
        x->previous = a;
        x->next = y;
        y->previous = x;
        b->next = c;
        if (a != NULL) {
          a->next = x;
        } else {
          first = x;
        }
        if (c != NULL) {
          c->previous = b;
        } else {
          last = b;
        }
      }
      x = next;
    }
  }

  ___CBTPOP;
}
