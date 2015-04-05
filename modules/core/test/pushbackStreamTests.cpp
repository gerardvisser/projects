/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2014 Gerard Visser.

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

#include <lombok/io/pushbackStream.h>
#include <lombok/testing/testUtils.h>
#include "pushbackStreamTests.h"
#include "testInputStream.h"

#define PRINT_TEST printTestedMethod ("pushbackStream_t")

static void testClose (void) {
  ___BTPUSH;
  PRINT_TEST;

  testInputStream_t* testInputStream = new testInputStream_t ();
  pushbackStream_t* stream = new pushbackStream_t (testInputStream);
  assert (!testInputStream->closeWasCalled ());
  stream->close ();
  assert (testInputStream->closeWasCalled ());
  stream->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testGetErrorCode (void) {
  ___BTPUSH;
  PRINT_TEST;

  testInputStream_t* testInputStream = new testInputStream_t ();
  pushbackStream_t* stream = new pushbackStream_t (testInputStream);
  testInputStream->setErrorCode (1);
  assert (stream->getErrorCode () == 1);
  testInputStream->setErrorCode (2);
  assert (stream->getErrorCode () == 2);
  stream->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testIsByteStream (void) {
  ___BTPUSH;
  PRINT_TEST;

  testInputStream_t* testInputStream = new testInputStream_t ();
  pushbackStream_t* stream = new pushbackStream_t (testInputStream);
  testInputStream->setByteStream (false);
  assert (!stream->isByteStream ());
  testInputStream->setByteStream (true);
  assert (stream->isByteStream ());
  stream->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testRead (void) {
  ___BTPUSH;
  PRINT_TEST;

  pushbackStream_t* stream = new pushbackStream_t (new testInputStream_t ());
  for (int i = 1; i <= 10; ++i) {
    assert (stream->read () == i);
  }
  assert (stream->read () == -1);
  stream->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testUnread (void) {
  ___BTPUSH;
  PRINT_TEST;

  pushbackStream_t* stream = new pushbackStream_t (new testInputStream_t ());
  for (int i = 1; i <= 10; ++i) {
    assert (stream->read () == i);
    stream->unread (i);
    assert (stream->read () == i);
  }
  assert (stream->read () == -1);
  stream->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

const struct pushbackStreamTestsStruct pushbackStreamTests = {
  testClose,
  testGetErrorCode,
  testIsByteStream,
  testRead,
  testUnread
};
