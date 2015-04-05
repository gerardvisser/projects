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
#include <lombok/testing/testUtils.h>
#include <lombok/util/string.h>
#include "fileInputStreamTests.h"
#include "testinput.h"
#include "xsperms.h"

#define PRINT_TEST printTestedMethod ("fileInputStream_t")

static void testConstructor (void) {
  ___BTPUSH;
  printTest ("fileInputStream_t::fileInputStream_t");

  fileInputStream_t* stream = new fileInputStream_t ("nietbestaand");
  assert (stream->getErrorCode () == IO_ERR__NO_SUCH_FILE);
  stream->dump ();

  xsperms_chmod (TESTINPUT_NAME, 0);
  stream = new fileInputStream_t (TESTINPUT_NAME);
  assert (stream->getErrorCode () == IO_ERR__NO_READ_PERMISSION);
  stream->dump ();

  xsperms_chmod (TESTINPUT_NAME, 0444); /* Make sure there is no write permission: file will also be used in output stream test. */
  stream = new fileInputStream_t (TESTINPUT_NAME);
  assert (stream->getErrorCode () == 0);
  stream->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testIsByteStream (void) {
  ___BTPUSH;
  PRINT_TEST;

  string_t* name = new string_t (TESTINPUT_NAME);
  fileInputStream_t* stream = new fileInputStream_t (name);
  name->dump ();
  assert (stream->isByteStream ());
  stream->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testRead (void) {
  ___BTPUSH;
  PRINT_TEST;

  int i;
  testinput_reset ();
  fileInputStream_t* stream = new fileInputStream_t (TESTINPUT_NAME);
  for (i = 0; i < TESTINPUT_SIZE + 1; ++i) {
    assert (stream->read () == testinput_nextByte ());
    assert (stream->getErrorCode () == 0);
  }
  stream->dump ();

  /* Now also test the overloaded read-method. */
  unsigned char buf[TESTINPUT_SIZE];
  stream = new fileInputStream_t (TESTINPUT_NAME);
  int numberOfBytesRead = stream->read (buf, 0x123);
  assert (numberOfBytesRead == 0x123);
  buf[numberOfBytesRead++] = stream->read ();
  buf[numberOfBytesRead++] = stream->read ();

  numberOfBytesRead += stream->read (buf + numberOfBytesRead, 0x123);
  assert (numberOfBytesRead == 0x248);
  buf[numberOfBytesRead++] = stream->read ();
  buf[numberOfBytesRead++] = stream->read ();
  buf[numberOfBytesRead++] = stream->read ();

  numberOfBytesRead += stream->read (buf + numberOfBytesRead, 0xB5);
  numberOfBytesRead += stream->read (buf + numberOfBytesRead, -1);
  assert (numberOfBytesRead == 0x300);

  numberOfBytesRead += stream->read (buf + numberOfBytesRead, 0x550);
  assert (numberOfBytesRead == 0x850);

  numberOfBytesRead += stream->read (buf + numberOfBytesRead, TESTINPUT_SIZE);
  assert (numberOfBytesRead == TESTINPUT_SIZE);
  assert (stream->read () == -1);
  stream->dump ();

  testinput_reset ();
  for (i = 0; i < TESTINPUT_SIZE; ++i) {
    assert (buf[i] == testinput_nextByte ());
  }

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

const struct fileInputStreamTestsStruct fileInputStreamTests = {
  testConstructor,
  testIsByteStream,
  testRead
};
