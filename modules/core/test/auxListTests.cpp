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

#include "../src/lombok/base/auxList.h"
#include <lombok/testing/testUtils.h>
#include "auxListTests.h"
#include <stdarg.h>

typedef struct elementStruct element_t;

struct elementStruct {
  element_t* previous;
  element_t* next;
  int id;
};

#define NUMBER_OF_ELEMENTS 10

static auxListElem_t* elements[NUMBER_OF_ELEMENTS];

static void checkListContents (auxList_t* list, ...);
static void createElements (void);
static void deleteElement (auxListElem_t* elem);

static void testAllMethods (void) {
  ___BTPUSH;
  printTest ("All methods of auxList_t");

  createElements ();
  auxList_t* list = new auxList_t (deleteElement);
  checkListContents (list, -1);

  list->appendToTail (elements[0]);
  assert (elements[0]->previous == NULL);
  assert (elements[0]->next == NULL);
  checkListContents (list, 0, -1);

  list->remove (elements[0]);
  checkListContents (list, -1);

  list->appendToHead (elements[4]);
  assert (elements[4]->previous == NULL);
  assert (elements[4]->next == NULL);
  checkListContents (list, 4, -1);

  list->appendToHead (elements[9]);
  checkListContents (list, 9, 4, -1);

  list->appendToTail (elements[2]);
  checkListContents (list, 9, 4, 2, -1);

  list->appendToHead (elements[3]);
  checkListContents (list, 3, 9, 4, 2, -1);

  list->appendToHead (elements[8]);
  checkListContents (list, 8, 3, 9, 4, 2, -1);

  list->appendToTail (elements[5]);
  checkListContents (list, 8, 3, 9, 4, 2, 5, -1);

  list->appendToTail (elements[6]);
  checkListContents (list, 8, 3, 9, 4, 2, 5, 6, -1);

  list->appendToTail (elements[0]);
  checkListContents (list, 8, 3, 9, 4, 2, 5, 6, 0, -1);

  list->remove (elements[9]);
  checkListContents (list, 8, 3, 4, 2, 5, 6, 0, -1);

  list->remove (elements[8]);
  checkListContents (list, 3, 4, 2, 5, 6, 0, -1);

  list->appendToHead (elements[9]);
  checkListContents (list, 9, 3, 4, 2, 5, 6, 0, -1);

  list->remove (elements[2]);
  checkListContents (list, 9, 3, 4, 5, 6, 0, -1);

  list->appendToHead (elements[2]);
  checkListContents (list, 2, 9, 3, 4, 5, 6, 0, -1);

  list->appendToHead (elements[1]);
  checkListContents (list, 1, 2, 9, 3, 4, 5, 6, 0, -1);

  list->remove (elements[0]);
  checkListContents (list, 1, 2, 9, 3, 4, 5, 6, -1);

  list->appendToHead (elements[0]);
  checkListContents (list, 0, 1, 2, 9, 3, 4, 5, 6, -1);

  list->appendToTail (elements[7]);
  checkListContents (list, 0, 1, 2, 9, 3, 4, 5, 6, 7, -1);

  list->appendToTail (elements[8]);
  checkListContents (list, 0, 1, 2, 9, 3, 4, 5, 6, 7, 8, -1);

  list->removeAndDelete (elements[9]);
  checkListContents (list, 0, 1, 2, 3, 4, 5, 6, 7, 8, -1);

  list->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

const struct auxListTestsStruct auxListTests = {
  testAllMethods
};

/* ... is a -1 terminated sequence of ints. */
static void checkListContents (auxList_t* list, ...) {
  ___BTPUSH;

  va_list argp;
  va_start (argp, list);

  int elemId = va_arg (argp, int);
  element_t* element = (element_t*) list->getHead ();
  while (element != NULL && elemId != -1 && element->id == elemId) {
    elemId = va_arg (argp, int);
    element = element->next;
  }
  assert (element == NULL && elemId == -1);

  va_end (argp);

  ___BTPOP;
}

static void createElements (void) {
  ___BTPUSH;

  for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i) {
    element_t* element = (element_t*) mem_allocate ("element_t", sizeof (element_t), PTYPE_ORDINARY);
    element->previous = (element_t*) NULL + 1;
    element->next = (element_t*) NULL + 1;
    element->id = i;
    elements[i] = (auxListElem_t*) element;
  }

  ___BTPOP;
}

static void deleteElement (auxListElem_t* elem) {
  ___BTPUSH;
  mem_release (elem);
  ___BTPOP;
}
