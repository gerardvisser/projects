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

#include <lombok/testing/testUtils.h>
#include "allTests.h"

void testRunner_runTests (void) {
  ___BTPUSH;

  run (intTests);
  run (ucharTests);
  run (objectTests);
  run (classTests);
  run (auxListTests);
  run (allocatorTests);

  run (arrayListTests);
  run (arrayListIteratorTests);
  run (linkedListTests);
  run (linkedListIteratorTests);
  run (hashMapTests);
  run (hashMapIteratorTests);
  run (stringTests);
  run (stringBuilderTests);
  run (charSequenceTests);

  run (filenameTests);
  run (fileInputStreamTests);
  run (fileOutputStreamTests);
  run (pushbackStreamTests);

  ___BTPOP;
}
