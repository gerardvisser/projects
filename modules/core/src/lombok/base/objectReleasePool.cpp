/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013, 2014 Gerard Visser.

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

#include <pthread.h>
#include <lombok/base/errors.h>
#include <lombok/base/objectReleasePool.h>
#include "auxList.h"

REFL_IMPL (objectReleasePool_t, object_t)

typedef struct elementStruct element_t;

struct elementStruct {
  auxListElem_t* previous;
  auxListElem_t* next;
  object_t* object;
};

static objectReleasePool_t* objectReleasePool = NULL;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static auxListElem_t* createElement (object_t* object);
static void deleteElement (auxListElem_t* elem);

objectReleasePool_t::objectReleasePool_t (void) {
  ___CBTPUSH;

  elements = new auxList_t (deleteElement);

  ___CBTPOP;
}

objectReleasePool_t::~objectReleasePool_t (void) {
  ___CBTPUSH;

  pthread_mutex_lock (&mutex);
  elements->dump ();
  objectReleasePool = NULL;
  pthread_mutex_unlock (&mutex);

  ___CBTPOP;
}

void objectReleasePool_t::add (allocator_t* obj) {
  ___CBTPUSH;

  pthread_mutex_lock (&mutex);
  ((auxList_t*) elements)->appendToTail (createElement (obj));
  pthread_mutex_unlock (&mutex);

  ___CBTPOP;
}

void objectReleasePool_t::add (object_t* obj) {
  ___CBTPUSH;

  pthread_mutex_lock (&mutex);
  ((auxList_t*) elements)->appendToHead (createElement (obj));
  pthread_mutex_unlock (&mutex);

  ___CBTPOP;
}

objectReleasePool_t* objectReleasePool_t::instance (void) {
  ___CBTPUSH;

  if (objectReleasePool == NULL) {
    pthread_mutex_lock (&mutex);
    if (objectReleasePool == NULL) {
      objectReleasePool = new objectReleasePool_t ();
    }
    pthread_mutex_unlock (&mutex);
  }

  ___CBTPOP;
  return objectReleasePool;
}

void* objectReleasePool_t::operator new (size_t s) {
  ___CBTPUSH;

  void* result = mem_allocate (NULL, s, PTYPE_ADMINISTRATION | PTYPE_REF_COUNT_CONTAINER);

  ___CBTPOP;
  return result;
}

static auxListElem_t* createElement (object_t* object) {
  ___BTPUSH;

  element_t* element = (element_t*) mem_allocate ("auxListElem_t", sizeof (element_t), PTYPE_ADMINISTRATION);
  element->object = object;

  ___BTPOP;
  return (auxListElem_t*) element;
}

static void deleteElement (auxListElem_t* elem) {
  ___BTPUSH;

  ((element_t*) elem)->object->dump ();
  mem_release (elem);

  ___BTPOP;
}
