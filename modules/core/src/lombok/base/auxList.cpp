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

#include <lombok/base/errors.h>
#include <lombok/base/mem.h>
#include "auxList.h"

REFL_IMPL (auxList_t, object_t)

auxList_t::auxList_t (void (* deleteElem)(auxListElem_t* elem)) : deleteElem (deleteElem), head (NULL), tail (NULL) {
}

auxList_t::~auxList_t (void) {
  ___CBTPUSH;

  auxListElem_t* elem = head;
  while (elem != NULL) {
    auxListElem_t* next = elem->next;
    deleteElem (elem);
    elem = next;
  }

  ___CBTPOP;
}

void auxList_t::appendToHead (auxListElem_t* elem) {
  ___CBTPUSH;

  if (head != NULL) {
    head->previous = elem;
  } else {
    tail = elem;
  }
  elem->previous = NULL;
  elem->next = head;
  head = elem;

  ___CBTPOP;
}

void auxList_t::appendToTail (auxListElem_t* elem) {
  ___CBTPUSH;

  if (tail != NULL) {
    tail->next = elem;
  } else {
    head = elem;
  }
  elem->previous = tail;
  elem->next = NULL;
  tail = elem;

  ___CBTPOP;
}

auxListElem_t* auxList_t::getHead (void) {
  return head;
}

void auxList_t::remove (auxListElem_t* elem) {
  ___CBTPUSH;

  if (elem->previous != NULL) {
    elem->previous->next = elem->next;
    if (elem->next != NULL) {
      elem->next->previous = elem->previous;
    } else {
      tail = elem->previous;
    }
  } else if (elem->next != NULL) {
    elem->next->previous = NULL;
    head = elem->next;
  } else {
    head = NULL;
    tail = NULL;
  }

  ___CBTPOP;
}

void auxList_t::removeAndDelete (auxListElem_t* elem) {
  ___CBTPUSH;

  remove (elem);
  deleteElem (elem);

  ___CBTPOP;
}

/* Operators: */

void* auxList_t::operator new (size_t s) {
  ___CBTPUSH;

  void* result = mem_allocate (NULL, s, PTYPE_ADMINISTRATION | PTYPE_REF_COUNT_CONTAINER);

  ___CBTPOP;
  return result;
}
