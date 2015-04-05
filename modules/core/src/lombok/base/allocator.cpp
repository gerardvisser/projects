/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

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
#include "auxList.h"

REFL_IMPL (allocator_t, object_t)

typedef struct blockStruct block_t;

struct blockStruct {
  block_t* previous;
  block_t* next;
  long int sp; /* Stack pointer. */
  char buf[];  /* Buffer. */
};

/* Layout of buffer:
   Let N be the number of a certain class of objects contained by
   the buffer and let S be the size of a single object of that class.
   Let R be the size of a `void*'.
   Offset:
   From:    To:
      0             N×R:  Stack of available objects (pointers).
    N×R     N×(2×R + S):  The actual objects, each preceded by a
                          pointer to the start of the block
                          (at offset: -3×R).

   This means that each object i starts at offset:
   R×(N + 1) + i×(S + R)
   where i in {0, 1, ..., N-1}     */

static void deleteBlock (auxListElem_t* block) {
  ___BTPUSH;
  mem_release (block);
  ___BTPOP;
}

#ifdef DEBUG_MODE

# define PERFORM_POINTER_TO_FREE_CHECKS(pointer) performPointerToFreeChecks (pointer)

allocator_t::allocator_t (long int objectSize, long int objectsPerBlock, long int ptype, const char* typeName) :
  objectSize (objectSize), objectsPerBlock (objectsPerBlock), ptype (ptype), typeName (typeName) {

  ___CBTPUSH;
  initialise ();
  ___CBTPOP;
}

void allocator_t::performPointerToFreeChecks (void* pointer) {
  ___CBTPUSH;
  block_t* block = (block_t*) ((void**) pointer)[-1];

  long denom = objectSize + sizeof (void*);
  long num = (long) pointer - sizeof (void*) * (objectsPerBlock + 1) - sizeof (block_t) - (long) block;
  if (num % denom != 0) {
    errors_printMessageAndExit ("Invalid pointer specified for object");
  }

  long indexUpperBound = block == newBlock ? this->index : objectsPerBlock;
  long index = num / denom;
  if (index < 0  ||  index >= indexUpperBound) {
    errors_printMessageAndExit ("Invalid pointer specified for object");
  }

  void** stack = (void**) block->buf;
  int i = 0;
  while (i < block->sp  &&  pointer != stack[i]) {
    ++i;
  }
  if (i < block->sp) {
    errors_printMessageAndExit ("Double free");
  }
  ___CBTPOP;
}

#else /* Not DEBUG_MODE */

# define PERFORM_POINTER_TO_FREE_CHECKS(pointer)

allocator_t::allocator_t (long int objectSize, long int objectsPerBlock) : objectSize (objectSize), objectsPerBlock (objectsPerBlock) {
  initialise ();
}

#endif /* DEBUG_MODE */



allocator_t::~allocator_t (void) {
  ___CBTPUSH;

  if (newBlock != NULL) {
    mem_release (newBlock);
  }
  partiallyUsedBlocks->dump ();
  fullyUsedBlocks->dump ();

  ___CBTPOP;
}

void* allocator_t::allocate (void) {
  ___CBTPUSH;
  void* object;
  if (newBlock != NULL) {
    object = getObjectFromNewBlock ();
  } else if (((auxList_t*) partiallyUsedBlocks)->getHead () != NULL) {
    object = getObjectFromHead ();
  } else {
    createNewBlock ();
    object = getObjectFromNewBlock ();
  }
  mem_register (typeName, objectSize, (int) ptype, object);
  ___CBTPOP;
  return object;
}

void allocator_t::createNewBlock (void) {
  ___CBTPUSH;
  /* size = 3×R + N×(2×R + S), where sizeof(block_t) = 3×R.  */
  long int size = sizeof (block_t) + objectsPerBlock * (2 * sizeof (void*) + objectSize);
  block_t* block = (block_t*) mem_allocate ("block_t", size, PTYPE_ADMINISTRATION);
  block->previous = NULL;
  block->next = NULL;
  block->sp = 0;
  newBlock = block;
  index = 0;
  ___CBTPOP;
}

void* allocator_t::getObjectFromHead (void) {
  ___CBTPUSH;
  block_t* block = (block_t*) ((auxList_t*) partiallyUsedBlocks)->getHead ();

  void** stack = (void**) block->buf;
  --block->sp;
  void* object = stack[block->sp];

  if (block->sp == 0) {
    ((auxList_t*) partiallyUsedBlocks)->remove ((auxListElem_t*) block);
    ((auxList_t*) fullyUsedBlocks)->appendToTail ((auxListElem_t*) block);
  }
  ___CBTPOP;
  return object;
}

void* allocator_t::getObjectFromNewBlock (void) {
  ___CBTPUSH;
  block_t* block = (block_t*) newBlock;
  /* offset = R×(N + 1) + i×(S + R) */
  long int offset = sizeof (void*) * (objectsPerBlock + 1) + index * (objectSize + sizeof (void*));
  void* object = block->buf + offset;
  ((void**) object)[-1] = block;
  ++index;
  if (index == objectsPerBlock) {
    newBlock = NULL;
    if (block->sp > 0) {
      ((auxList_t*) partiallyUsedBlocks)->appendToTail ((auxListElem_t*) block);
    } else {
      ((auxList_t*) fullyUsedBlocks)->appendToTail ((auxListElem_t*) block);
    }
  }
  ___CBTPOP;
  return object;
}

void allocator_t::initialise (void) {
  ___CBTPUSH;

  partiallyUsedBlocks = new auxList_t (deleteBlock);
  fullyUsedBlocks = new auxList_t (deleteBlock);
  newBlock = NULL;
  index = 0;

  ___CBTPOP;
}

void allocator_t::release (void* object) {
  ___CBTPUSH;
  PERFORM_POINTER_TO_FREE_CHECKS (object);
  mem_deregister (object);

  block_t* block = (block_t*) ((void**) object)[-1];

  void** stack = (void**) block->buf;
  stack[block->sp] = object;
  ++block->sp;

  if (block->sp == objectsPerBlock) {
    ((auxList_t*) partiallyUsedBlocks)->remove ((auxListElem_t*) block);
    mem_release (block);
  } else if (block->sp == 1 && block != newBlock) {
    ((auxList_t*) fullyUsedBlocks)->remove ((auxListElem_t*) block);
    ((auxList_t*) partiallyUsedBlocks)->appendToTail ((auxListElem_t*) block);
  }
  ___CBTPOP;
}

/* Operators: */

void* allocator_t::operator new (size_t s) {
  ___CBTPUSH;

  void* result = mem_allocate (NULL, s, PTYPE_ADMINISTRATION | PTYPE_REF_COUNT_CONTAINER);

  ___CBTPOP;
  return result;
}
