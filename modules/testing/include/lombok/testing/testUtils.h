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

#ifndef ___LOMBOK__TESTING__TEST_UTILS_INCLUDED
# define ___LOMBOK__TESTING__TEST_UTILS_INCLUDED

# include <lombok/base/all.h>
# include <assert.h>

/*

printTestedMethod
   Uses `printTest' to print the name of the tested method.
   Expects the name of the function to perform the test to start
   with `test' followed by the name of the method to be tested, of
   which the first letter is capitalised. For example: the function
   `testAdd' is the name for the function that tests method `add'.

 */

long unsigned int distance (const void* p1, const void* p2);
void printOk (void);
void printTest (const char* testedFunction);
void printTestedMethod (const char* namespaceName, const char* testFunctionName);
void printTestedMethod (object_t* objectTestIsPerformedOn, const char* testFunctionName);
void run (const void* testContainer, int numberOfTests);

# ifdef DEBUG_MODE

void performMemoryTest (const char* fileName, int line, long int expected = 0);
void performRefCountTest (object_t* obj, long int expected, const char* fileName, int line);

#  define performMemoryTest(...) performMemoryTest (__FILE__, __LINE__, ##__VA_ARGS__)
#  define performRefCountTest(obj, expected) performRefCountTest (obj, expected, __FILE__, __LINE__)

# else

#  define performMemoryTest(...)
#  define performRefCountTest(obj, expected)

# endif

# define printTestedMethod(objectOrNamespace) printTestedMethod (objectOrNamespace, __func__)
# define run(x) run (&x, sizeof (x)/sizeof (void*))

#endif
