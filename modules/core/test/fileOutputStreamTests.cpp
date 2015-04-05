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

#include <lombok/io/errorCodes.h>
#include <lombok/io/fileInputStream.h>
#include <lombok/io/fileOutputStream.h>
#include <lombok/testing/testUtils.h>
#include <lombok/util/string.h>
#include "fileOutputStreamTests.h"
#include "testinput.h"

#define PRINT_TEST printTestedMethod ("fileOutputStream_t")
#define TESTOUTPUT_NAME "testoutput"

static void testConstructor (void) {
  ___BTPUSH;
  printTest ("fileOutputStream_t::fileOutputStream_t");

  fileOutputStream_t* stream = new fileOutputStream_t (TESTINPUT_NAME);
  assert (stream->getErrorCode () == IO_ERR__NO_WRITE_PERMISSION);
  stream->dump ();

  stream = new fileOutputStream_t (".");
  assert (stream->getErrorCode () == IO_ERR__IS_DIRECTORY);
  stream->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testIsByteStream (void) {
  ___BTPUSH;
  PRINT_TEST;

  string_t* name = new string_t (TESTOUTPUT_NAME);
  fileOutputStream_t* stream = new fileOutputStream_t (name);
  name->dump ();
  assert (stream->getErrorCode () == 0);
  assert (stream->isByteStream ());
  stream->close ();
  stream->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testWrite (void) {
  ___BTPUSH;
  PRINT_TEST;

  assert (TESTINPUT_SIZE > 0x800);

  int i;
  testinput_reset ();
  fileOutputStream_t* stream = new fileOutputStream_t (TESTOUTPUT_NAME);
  for (i = 0; i < 300; ++i) {
    assert (stream->write (testinput_nextByte ()) == 0);
    assert (stream->getErrorCode () == 0);
  }
  char buf[0x800];
  for (i = 0; i < 1316; ++i) {
    buf[i] = testinput_nextByte ();
  }
  buf[i] = 0;
  assert (stream->write (buf) == 0);
  assert (stream->getErrorCode () == 0);
  for (i = 0; i < 400; ++i) {
    buf[i] = testinput_nextByte ();
  }
  assert (stream->write (buf, 400) == 0);
  assert (stream->getErrorCode () == 0);
  for (i = 0; i < TESTINPUT_SIZE - 2016; ++i) {
    assert (stream->write (testinput_nextByte ()) == 0);
    assert (stream->getErrorCode () == 0);
  }
  stream->dump (); /* Should close automatically. */

  testinput_reset ();
  fileInputStream_t* inputStream = new fileInputStream_t (TESTOUTPUT_NAME);
  for (int i = 0; i < TESTINPUT_SIZE + 1; ++i) {
    assert (inputStream->read () == testinput_nextByte ());
  }
  inputStream->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

const struct fileOutputStreamTestsStruct fileOutputStreamTests = {
  testConstructor,
  testIsByteStream,
  testWrite
};
