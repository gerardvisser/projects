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

#include "charSequenceTests.h"
#include <lombok/testing/testUtils.h>
#include <lombok/util/stringBuilder.h>

const uchar_t cs1[] = {0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048,
                       0x0049, 0x004A, 0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5,
                       0x00D6, 0x00D7, 0x0132, 0x0133, 0x2630, 0x2631, 0x2632, 0x2633,
                       0x2634, 0x2635, 0x2636, 0x2637, 0};
const uchar_t cs2[] = {0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078,
                       0x0079, 0x007A, 0};
const uchar_t cs3[] = {0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078,
                       0x0079, 0x005A, 0};

static void checkUtf8 (const char* utf8, const uchar_t* unicode, int utf8Length = 0) {
  ___BTPUSH;

  int i = 0;
  int j = 0;
  while (unicode[i] != 0) {
    int x = uchar_toUtf8 (unicode[i]);
    while (x > 0) {
      assert ((utf8[j] & 0xFF) == (x & 0xFF));
      x >>= 8;
      ++j;
    }
    ++i;
  }
  if (utf8Length == 0) {
    assert (utf8[j] == 0);
  } else {
    assert (j == utf8Length);
  }

  ___BTPOP;
}

static int hashCode (const uchar_t* str) {
  ___BTPUSH;

  int hash = charsequenceTests::length (str);
  int max = hash < 8 ? hash : 8;
  for (int i = 0; i < max; ++i) {
    hash += (int) str[i] << i;
  }

  ___BTPOP;
  return hashCode_get (hash);
}

int charsequenceTests::length (const uchar_t* str) {
  ___BTPUSH;

  int len = 0;
  while (str[len] != 0) {
    ++len;
  }

  ___BTPOP;
  return len;
}

void charsequenceTests::testCharAt (charSequence_t* (* newCharSequence)(const uchar_t* str)) {
  ___BTPUSH;
  charSequence_t* str = newCharSequence (cs1);
  printTestedMethod (str);

  int len = length (cs1);
  for (int i = 0; i < len; ++i) {
    assert (str->charAt (i) == cs1[i]);
  }

  str->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void charsequenceTests::testEquals (charSequence_t* (* newCharSequence)(const uchar_t* str)) {
  ___BTPUSH;
  charSequence_t* str = newCharSequence (cs2);
  printTestedMethod (str);

  charSequence_t* str2 = newCharSequence (cs2);
  charSequence_t* str3 = newCharSequence (cs3);

  assert (str->equals (str));
  assert (str->equals (str2));
  assert (!str->equals (str3));
  assert (str2->equals (str));
  assert (str2->equals (str2));
  assert (!str2->equals (str3));
  assert (!str3->equals (str));
  assert (!str3->equals (str2));
  assert (str3->equals (str3));

  str->dump ();
  str2->dump ();
  str3->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void charsequenceTests::testHashCode (charSequence_t* (* newCharSequence)(const uchar_t* str)) {
  ___BTPUSH;
  charSequence_t* str = newCharSequence (cs1);
  printTestedMethod (str);

  assert (str->hashCode () == hashCode (cs1));
  str->dump ();
  str = newCharSequence (cs2);
  assert (str->hashCode () == hashCode (cs2));
  str->dump ();
  str = newCharSequence (cs3);
  assert (str->hashCode () == hashCode (cs3));

  str->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void charsequenceTests::testLength (charSequence_t* (* newCharSequence)(const uchar_t* str)) {
  ___BTPUSH;
  charSequence_t* str = newCharSequence (cs1);
  printTestedMethod (str);

  assert (str->length () == length (cs1));
  str->dump ();
  str = newCharSequence (cs2);
  assert (str->length () == length (cs2));

  str->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

void charsequenceTests::testToUtf8 (charSequence_t* (* newCharSequence)(const uchar_t* str)) {
  ___BTPUSH;
  charSequence_t* str = newCharSequence (cs1);
  printTestedMethod (str);

  char buf[256];
  int bytesWritten;

  char* utf8 = str->toUtf8 ();
  checkUtf8 (utf8, cs1);
  mem_release (utf8);

  utf8 = str->toUtf8 (buf, 48);
  checkUtf8 (utf8, cs1);
  assert (utf8 != buf);
  mem_release (utf8);

  utf8 = str->toUtf8 (buf, 256, &bytesWritten);
  checkUtf8 (utf8, cs1, bytesWritten);
  assert (utf8 == buf);

  str->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testCompare (void) {
  ___BTPUSH;
  printTest ("charSequence_t::compare");

  string_t* bad = new string_t ("bad");
  string_t* bel = new string_t ("bel");
  string_t* bezem = new string_t ("bezem");
  string_t* bellen = new string_t ("bellen");
  stringBuilder_t* bellen2 = new stringBuilder_t ();
  bellen2->append (bellen);

  assert (charSequence_t::compare (bad, bad) == 0);
  assert (charSequence_t::compare (bad, bel) < 0);
  assert (charSequence_t::compare (bad, bellen) < 0);
  assert (charSequence_t::compare (bad, bellen2) < 0);
  assert (charSequence_t::compare (bad, bezem) < 0);

  assert (charSequence_t::compare (bel, bad) > 0);
  assert (charSequence_t::compare (bel, bel) == 0);
  assert (charSequence_t::compare (bel, bellen) < 0);
  assert (charSequence_t::compare (bel, bellen2) < 0);
  assert (charSequence_t::compare (bel, bezem) < 0);

  assert (charSequence_t::compare (bellen, bad) > 0);
  assert (charSequence_t::compare (bellen, bel) > 0);
  assert (charSequence_t::compare (bellen, bellen) == 0);
  assert (charSequence_t::compare (bellen, bellen2) == 0);
  assert (charSequence_t::compare (bellen, bezem) < 0);

  assert (charSequence_t::compare (bellen2, bad) > 0);
  assert (charSequence_t::compare (bellen2, bel) > 0);
  assert (charSequence_t::compare (bellen2, bellen) == 0);
  assert (charSequence_t::compare (bellen2, bellen2) == 0);
  assert (charSequence_t::compare (bellen2, bezem) < 0);

  assert (charSequence_t::compare (bezem, bad) > 0);
  assert (charSequence_t::compare (bezem, bel) > 0);
  assert (charSequence_t::compare (bezem, bellen) > 0);
  assert (charSequence_t::compare (bezem, bellen2) > 0);
  assert (charSequence_t::compare (bezem, bezem) == 0);

  bad->dump ();
  bel->dump ();
  bezem->dump ();
  bellen->dump ();
  bellen2->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

const struct charSequenceTestsStruct charSequenceTests = {
  testCompare
};
