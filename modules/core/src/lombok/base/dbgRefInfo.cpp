/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2008, 2009, 2010, 2011, 2012, 2013 Gerard Visser.

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

#ifdef DEBUG_MODE

#include <stdlib.h>
#include "dbgRefInfo.h"
#include <lombok/base/mem.h>
#include <lombok/base/class.h>

dbgRefInfo_t::dbgRefInfo_t (const void* pointer, const char* typeName, long int size, int ptype) : pointer (pointer), typeName (typeName), size (size), ptype (ptype) {
  previous = NULL;
  next = NULL;
}

dbgRefInfo_t::~dbgRefInfo_t (void) {
  if ((ptype & PTYPE_VIRTUAL) == 0) {
    free ((void*) pointer);
  }
}

dbgRefInfo_t* dbgRefInfo_t::getNext (void) {
  return next;
}

const void* dbgRefInfo_t::getPointer (void) {
  return pointer;
}

dbgRefInfo_t* dbgRefInfo_t::getPrevious (void) {
  return previous;
}

int dbgRefInfo_t::getPtype (void) {
  return ptype;
}

long int dbgRefInfo_t::getRefCount (void) {
  long int refCount;

  if ((ptype & PTYPE_REF_COUNT_CONTAINER) != 0) {
    refCount = ((object_t*) pointer)->getRefCount ();
  } else {
    refCount = 1;
  }
  return refCount;
}

long int dbgRefInfo_t::getSize (void) {
  return size;
}

const char* dbgRefInfo_t::getTypeName (void) {
  const char* name;

  if (typeName == NULL) {
    if ((ptype & PTYPE_REF_COUNT_CONTAINER) != 0) {
      name = ((object_t*) pointer)->getClass ()->name;
    } else {
      name = "(Unknown type)";
    }
  } else {
    name = typeName;
  }
  return name;
}

void dbgRefInfo_t::setNext (dbgRefInfo_t* val) {
  next = val;
}

void dbgRefInfo_t::setPointer (const void* val) {
  pointer = val;
}

void dbgRefInfo_t::setPrevious (dbgRefInfo_t* val) {
  previous = val;
}

void dbgRefInfo_t::setSize (long int val) {
  size = val;
}

#endif
