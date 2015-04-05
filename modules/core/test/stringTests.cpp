/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013 Gerard Visser.

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
#include <lombok/util/string.h>
#include "charSequenceTests.h"
#include "stringTests.h"

static charSequence_t* newCharSequence (const uchar_t* str) {
  ___BTPUSH;

  int len = charsequenceTests::length (str);
  string_t* result = new string_t (str, len);

  ___BTPOP;
  return result;
}

static void testCharAt (void) {
  ___BTPUSH;
  charsequenceTests::testCharAt (newCharSequence);
  ___BTPOP;
}

static void testConstructor (void) {
  ___BTPUSH;
  printTest ("string_t::string_t");

  /* Fill a buffer with the characters 0020 - 1000, utf8 encoded. */
  const int bufSize = 0x60 + 2 * 0x780 + 3 * 0x800 + 1;
  char buf[bufSize];
  int i, j = 0;
  for (i = 0x20; i < 0x1000; ++i) {
    int x = uchar_toUtf8 (i);
    while (x > 0) {
      buf[j] = x;
      x >>= 8;
      ++j;
    }
  }
  buf[j] = 0;

  /* Check whether the string contains exactly those characters. */
  string_t* str = new string_t (buf);
  assert (str->length () == 0xFE0);
  for (i = 0; i < 0xFE0; ++i) {
    assert (str->charAt (i) == i + 0x20);
  }
  str->dump ();

  /* Fill the buffer with the characters 0020 - 0100, ISO 8859-1 encoded. */
  for (i = 0x60; i < 0xE0; ++i) {
    buf[i] = i + 0x20;
  }
  buf[i] = 0;

  /* Check whether the string contains exactly those characters. */
  str = new string_t (buf, CHAR_ENC_ISO8859_1);
  assert (str->length () == 0xE0);
  for (i = 0; i < 0xE0; ++i) {
    assert (str->charAt (i) == i + 0x20);
  }
  str->dump ();

  /* Fill the buffer with some characters, ISO 8859-2 encoded. */
  buf[0x5F] = 0xA3;
  buf[0x60] = 0xA9;
  buf[0x61] = 0xB1;
  buf[0x62] = 0xBC;
  buf[0x63] = 0xBD;
  buf[0x64] = 0xC4;
  buf[0x65] = 0xD8;
  buf[0x66] = 0xE3;
  buf[0x67] = 0xF9;
  buf[0x68] = 0xFE;
  buf[0x69] = 0;

  /* Check whether the string contains exactly those characters. */
  str = new string_t (buf, CHAR_ENC_ISO8859_2);
  assert (str->length () == 0x69);
  for (i = 0; i < 0x5F; ++i) {
    assert (str->charAt (i) == i + 0x20);
  }
  assert (str->charAt (0x5F) == 0x141);
  assert (str->charAt (0x60) == 0x160);
  assert (str->charAt (0x61) == 0x105);
  assert (str->charAt (0x62) == 0x17A);
  assert (str->charAt (0x63) == 0x2DD);
  assert (str->charAt (0x64) == 0xC4);
  assert (str->charAt (0x65) == 0x158);
  assert (str->charAt (0x66) == 0x103);
  assert (str->charAt (0x67) == 0x16F);
  assert (str->charAt (0x68) == 0x163);
  str->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testEquals (void) {
  ___BTPUSH;
  charsequenceTests::testEquals (newCharSequence);
  ___BTPOP;
}

static void testHashCode (void) {
  ___BTPUSH;
  charsequenceTests::testHashCode (newCharSequence);
  ___BTPOP;
}

static void testLength (void) {
  ___BTPUSH;
  charsequenceTests::testLength (newCharSequence);
  ___BTPOP;
}

#define TEST_SUBSTRING(index, length, result) \
  t = s->substring (index, length); \
  u = new string_t (result); \
  assert (t->equals (u)); \
  t->dump (); \
  u->dump ()

static void testSubstring (void) {
  ___BTPUSH;
  printTestedMethod ("string_t");

  string_t* s = new string_t ("abcdefgh");
  string_t* t, * u;

  TEST_SUBSTRING (0, s->length (), "abcdefgh");
  TEST_SUBSTRING (s->length (), 0, "");
  TEST_SUBSTRING (3, 1, "d");
  TEST_SUBSTRING (4, 3, "efg");
  TEST_SUBSTRING (1, 7, "bcdefgh");

  s->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testToUtf8 (void) {
  ___BTPUSH;
  charsequenceTests::testToUtf8 (newCharSequence);
  ___BTPOP;
}

const struct stringTestsStruct stringTests = {
  testCharAt,
  testEquals,
  testHashCode,
  testLength,
  testToUtf8,
  testConstructor,
  testSubstring
};
