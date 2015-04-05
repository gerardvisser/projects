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

#include <lombok/util/stringBuilder.h>
#include <lombok/testing/testUtils.h>
#include "stringBuilderTests.h"
#include "charSequenceTests.h"

static charSequence_t* newCharSequence (const uchar_t* str) {
  ___BTPUSH;

  int i = 0;
  stringBuilder_t* result = new stringBuilder_t (0x100);
  while (str[i] != 0) {
    result->append (str[i]);
    ++i;
  }

  ___BTPOP;
  return result;
}

static void testAppend (void) {
  ___BTPUSH;
  stringBuilder_t* sb = new stringBuilder_t (8);
  printTestedMethod (sb);

  string_t* str = new string_t ("abcdefg");
  sb->append (str);
  str->dump ();
  str = new string_t ("hijklmnopqrstuvwxyz");
  sb->append (str);
  str->dump ();

  int i;
  for (i = 0; i < 4; ++i) {
    sb->append ('A' + i);
  }
  for (i = 0; i < 5; ++i) {
    sb->append ('E' + i, i);
  }

  assert (sb->length () == 40);
  for (i = 0; i < 26; ++i) {
    assert (sb->charAt (i) == 'a' + i);
  }
  for (i = 0; i < 4; ++i) {
    assert (sb->charAt (i + 26) == 'A' + i);
  }
  for (i = 0; i < 4; ++i) {
    for (int j = 0; j < i + 1; ++j) {
      assert (sb->charAt (30 + i * (i + 1) / 2 + j) == 'F' + i);
    }
  }

  sb->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testCharAt (void) {
  ___BTPUSH;
  charsequenceTests::testCharAt (newCharSequence);
  ___BTPOP;
}

static void testClear (void) {
  ___BTPUSH;
  stringBuilder_t* sb = new stringBuilder_t ();
  printTestedMethod (sb);

  string_t* str = new string_t ("abcdefg");
  sb->append (str);
  str->dump ();
  assert (sb->length () == 7);
  sb->clear ();
  assert (sb->length () == 0);

  sb->dump ();
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

static void testRemoveLastCharacter (void) {
  ___BTPUSH;
  stringBuilder_t* sb = new stringBuilder_t ();
  printTestedMethod (sb);

  string_t* str = new string_t ("abcdefg");
  sb->append (str);
  str->dump ();
  str = new string_t ("abcdef");
  sb->removeLastCharacter ();
  assert (sb->equals (str));
  str->dump ();

  sb->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testToString (void) {
  ___BTPUSH;
  stringBuilder_t* sb = new stringBuilder_t ();
  printTestedMethod (sb);

  string_t* str = new string_t ("abcdefg");
  sb->append (str);
  str->dump ();
  str = sb->toString ();
  assert (sb->equals (str));
  str->dump ();

  sb->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testToUtf8 (void) {
  ___BTPUSH;
  charsequenceTests::testToUtf8 (newCharSequence);
  ___BTPOP;
}

const struct stringBuilderTestsStruct stringBuilderTests = {
  testCharAt,
  testEquals,
  testHashCode,
  testLength,
  testToUtf8,
  testAppend,
  testClear,
  testRemoveLastCharacter,
  testToString
};
