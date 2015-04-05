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

#include <lombok/testing/testUtils.h>
#include "allocatorTests.h"
#include "buffer.h"
#include <stdarg.h>

#undef performMemoryTest
#define performMemoryTest(x) performAllocatorMemoryTest (__FILE__, __LINE__, x)

#define OBJECT_SIZE       12
#define OBJECTS_PER_BLOCK 4  /* Changing this value makes the test fail! */

static void createObjects (allocator_t* allocator, buffer_t* buffer, int numberOfObjectsToCreate);
static void deleteObjects (allocator_t* allocator, buffer_t* buffer, ...);
static void performAllocatorMemoryTest (const char* fileName, int line, long int expected);

static void testAllMethods (void) {
  ___BTPUSH;
  printTest ("All methods of allocator_t");

  buffer_t* buffer = new buffer_t (32);
  allocator_t* allocator = new allocator_t (OBJECT_SIZE, OBJECTS_PER_BLOCK, PTYPE_ORDINARY, "Test type");
  const int baseMem = mem_currentlyAllocatedBytes_real ();

  createObjects (allocator, buffer, 1);
  int expected = mem_currentlyAllocatedBytes_real ();
  const int blockSize = expected - baseMem;
  assert (blockSize >= OBJECTS_PER_BLOCK * OBJECT_SIZE);
  createObjects (allocator, buffer, OBJECTS_PER_BLOCK - 1);
  performMemoryTest (expected);
  deleteObjects (allocator, buffer, 0, -1);
  performMemoryTest (expected);
  createObjects (allocator, buffer, 1);
  performMemoryTest (expected);

  expected += blockSize;
  createObjects (allocator, buffer, 1);
  performMemoryTest (expected);

  expected += 4*blockSize;
  createObjects (allocator, buffer, 5 * OBJECTS_PER_BLOCK - 2);
  performMemoryTest (expected);

  deleteObjects (allocator, buffer, OBJECTS_PER_BLOCK + 1,
                                    2,
                                    3 * OBJECTS_PER_BLOCK,
                                    3 * OBJECTS_PER_BLOCK + 2,
                                    OBJECTS_PER_BLOCK,
                                    2 * OBJECTS_PER_BLOCK + 1,
                                    0,
                                    OBJECTS_PER_BLOCK + 2,
                                    5 * OBJECTS_PER_BLOCK,
                                    -1);
  performMemoryTest (expected);

  expected -= blockSize;
  deleteObjects (allocator, buffer, 2, -1);
  performMemoryTest (expected);

  /* 23 (6*OBJECTS_PER_BLOCK-1) had been created of which were then 10 deleted.
     Four of those objects were from the same block which resulted in the block
     being freed. It is therefore expected that the remaining 6 plus 1 from the
     sixth block which wasn't used yet, can be created without any extra memory
     to be allocated.
        Note that buffer now has a length of 13.  */
  createObjects (allocator, buffer, 7);
  performMemoryTest (expected);

  /* Creating one more object causes another block of memory to be allocated. */
  expected += blockSize;
  createObjects (allocator, buffer, 1);
  performMemoryTest (expected);

  deleteObjects (allocator, buffer, 13, 14, 15, 16, 17, 18, 19, 20, -1);
  performMemoryTest (expected);


  allocator->dump ();
  /* deregister all that is left in buffer. */
  for (int i = 0; i < buffer->length (); ++i) {
    mem_deregister (buffer->get (i));
  }
  delete buffer;
  performMemoryTest (0);

  printOk ();
  ___BTPOP;
}

const struct allocatorTestsStruct allocatorTests = {
  testAllMethods
};

static bool noOverlap (buffer_t* buffer, void* obj);

/* To be used with `qsort' (stdlib.h).
   This comparison function induces a sorting from largest to smallest. */
static int compareInts (const void* x, const void* y) {
  return ((const int*) y)[0] - ((const int*) x)[0];
}

/* Uses allocator to create `numberOfObjectsToCreate' objects and stores these
   in the supplied buffer. For each created object it is asserted that there
   is no overlap with previously created objects. */
static void createObjects (allocator_t* allocator, buffer_t* buffer, int numberOfObjectsToCreate) {
  ___BTPUSH;

  for (int i = 0; i < numberOfObjectsToCreate; ++i) {
    void* obj = allocator->allocate ();
    assert (noOverlap (buffer, obj));
    buffer->add (obj);
  }

  ___BTPOP;
}

#define MAX_TO_DELETE_PER_CALL 32

/* Buffer contains objects created with allocator.
   ... is a -1 terminated sequence of ints which are the indices of objects
   within buffer that should be deleted. At most MAX_TO_DELETE_PER_CALL indices
   per call can be handled. */
static void deleteObjects (allocator_t* allocator, buffer_t* buffer, ...) {
  ___BTPUSH;

  int n = 0;
  int indices[MAX_TO_DELETE_PER_CALL];
  va_list argp;

  va_start (argp, buffer);

  int index = va_arg (argp, int);
  while (n < MAX_TO_DELETE_PER_CALL && index != -1) {
    allocator->release (buffer->get (index));
    indices[n] = index;
    index = va_arg (argp, int);
    ++n;
  }
  va_end (argp);

  qsort (indices, n, sizeof (int), compareInts);
  for (int i = 0; i < n; ++i) {
    buffer->remove (indices[i]);
  }

  ___BTPOP;
}

static bool noOverlap (buffer_t* buffer, void* obj) {
  ___BTPUSH;
  int i = 0;
  while (i < buffer->length () && distance (obj, buffer->get (i)) >= OBJECT_SIZE) {
    ++i;
  }
  ___BTPOP;
  return i == buffer->length ();
}

static void performAllocatorMemoryTest (const char* fileName, int line, long int expected) {
  ___BTPUSH;
  long int currentlyAllocated = mem_currentlyAllocatedBytes_real ();
  if (currentlyAllocated != expected) {
    errors_printMessageAndExit ("Memory test failed (file: %s, line: %d).\nExpected %Z bytes to be allocated; actual: %Z.", fileName, line, expected, currentlyAllocated);
  }
  ___BTPOP;
}
