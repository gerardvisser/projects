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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <lombok/testing/testUtils.h>

#undef performRefCountTest
#undef performMemoryTest
#undef printTestedMethod
#undef run

long unsigned int distance (const void* p1, const void* p2) {
  long unsigned int x, y;

  if ((unsigned long) p1 > (unsigned long) p2) {
    y = (long) p1;
    x = (long) p2;
  } else {
    y = (long) p2;
    x = (long) p1;
  }
  return y - x;
}

#ifdef DEBUG_MODE

void performMemoryTest (const char* fileName, int line, long int expected) {
  ___BTPUSH;
  long int currentlyAllocated = mem_currentlyAllocatedBytes ();
  if (currentlyAllocated != expected) {
    errors_printMessageAndExit ("Memory test failed (file: %s, line: %d).\nExpected %Z bytes to be allocated; actual: %Z.", fileName, line, expected, currentlyAllocated);
  }
  ___BTPOP;
}

void performRefCountTest (object_t* obj, long int expected, const char* fileName, int line) {
  ___BTPUSH;
  long int actual = obj->getRefCount ();
  if (actual != expected) {
    errors_printMessageAndExit ("Ref-count test failed (file: %s, line: %d).\nExpected %Z, actual: %Z.", fileName, line, expected, actual);
  }
  ___BTPOP;
}

#endif

void printOk (void) {
  printf ("\x1B[1;32mOk\x1B[0m\n");
}

void printTest (const char* testedFunction) {
  ___BTPUSH;
  printf ("Tested: %s. Result: ", testedFunction);
  ___BTPOP;
}

void printTestedMethod (const char* namespaceName, const char* testFunctionName) {
  ___BTPUSH;
  char tested[512];
  char* methodName = stpcpy (stpcpy (tested, namespaceName), "::");
  methodName[0] = tolower (testFunctionName[4]);
  stpcpy (methodName + 1, testFunctionName + 5);
  printTest (tested);
  ___BTPOP;
}

void printTestedMethod (object_t* objectTestIsPerformedOn, const char* testFunctionName) {
  ___BTPUSH;
  printTestedMethod (objectTestIsPerformedOn->getClass ()->name, testFunctionName);
  ___BTPOP;
}

void run (const void* testContainer, int numberOfTests) {
  ___BTPUSH;
  void (** test)(void) = (void (**)(void)) testContainer;
  for (int i = 0; i < numberOfTests; ++i) {
    test[i] ();
  }
  ___BTPOP;
}
