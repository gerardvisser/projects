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

#ifndef ___LOMBOK__BASE__ALLOCATOR_INCLUDED
#define ___LOMBOK__BASE__ALLOCATOR_INCLUDED

#include <lombok/base/class.h>
#include <lombok/base/errors.h>
#include <lombok/base/mem.h>

#ifdef DEBUG_MODE

# define INIT_ALLOCATOR(identifier, objectSize, objectsPerBlock, ...) \
  static allocator_t* identifier = NULL; \
  static allocator_t* getAllocator (void) { \
    ___BTPUSH; \
    if (identifier == NULL) { \
      identifier = new allocator_t (objectSize, objectsPerBlock, ## __VA_ARGS__); \
      objectReleasePool_t::instance ()->add (identifier); \
    } \
    ___BTPOP; \
    return identifier; \
  }

#else

# define INIT_ALLOCATOR(identifier, objectSize, objectsPerBlock, ...) \
  static allocator_t* identifier = NULL; \
  static allocator_t* getAllocator (void) { \
    if (identifier == NULL) { \
      identifier = new allocator_t (objectSize, objectsPerBlock); \
      objectReleasePool_t::instance ()->add (identifier); \
    } \
    return identifier; \
  }

#endif

class allocator_t : public object_t {
private:
#ifdef DEBUG_MODE
  const char* const typeName;
  const long int ptype;
#endif
  const long int objectSize;
  const long int objectsPerBlock;
  object_t* partiallyUsedBlocks;
  object_t* fullyUsedBlocks;
  long int index;
  void* newBlock;

public:
#ifdef DEBUG_MODE
  allocator_t (long int objectSize, long int objectsPerBlock, long int ptype = PTYPE_REF_COUNT_CONTAINER, const char* typeName = NULL);
#else
  allocator_t (long int objectSize, long int objectsPerBlock);
#endif
  virtual ~allocator_t (void);

  virtual void* allocate (void);
  virtual void  release (void* object);

private:
  void  createNewBlock (void);
  void* getObjectFromHead (void);
  void* getObjectFromNewBlock (void);
  void  initialise (void);

#ifdef DEBUG_MODE
  void  performPointerToFreeChecks (void* pointer);
#endif

  REFL_DECL

  static void* operator new (size_t s);
};

#include <lombok/base/objectReleasePool.h>

#endif
