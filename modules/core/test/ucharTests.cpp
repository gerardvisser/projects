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
#include "ucharTests.h"

static int toUtf8 (uchar_t cr);

static void testHexDigitValue (void) {
  ___BTPUSH;
  printTest ("uchar_hexDigitValue");

  assert (uchar_hexDigitValue ('0') == 0);
  assert (uchar_hexDigitValue ('1') == 1);
  assert (uchar_hexDigitValue ('2') == 2);
  assert (uchar_hexDigitValue ('3') == 3);
  assert (uchar_hexDigitValue ('4') == 4);
  assert (uchar_hexDigitValue ('5') == 5);
  assert (uchar_hexDigitValue ('6') == 6);
  assert (uchar_hexDigitValue ('7') == 7);
  assert (uchar_hexDigitValue ('8') == 8);
  assert (uchar_hexDigitValue ('9') == 9);
  assert (uchar_hexDigitValue ('A') == 10);
  assert (uchar_hexDigitValue ('B') == 11);
  assert (uchar_hexDigitValue ('C') == 12);
  assert (uchar_hexDigitValue ('D') == 13);
  assert (uchar_hexDigitValue ('E') == 14);
  assert (uchar_hexDigitValue ('F') == 15);
  assert (uchar_hexDigitValue ('a') == 10);
  assert (uchar_hexDigitValue ('b') == 11);
  assert (uchar_hexDigitValue ('c') == 12);
  assert (uchar_hexDigitValue ('d') == 13);
  assert (uchar_hexDigitValue ('e') == 14);
  assert (uchar_hexDigitValue ('f') == 15);

  printOk ();
  ___BTPOP;
}

static void testIsBinDigit (void) {
  ___BTPUSH;
  printTest ("uchar_isBinDigit");

  for (int i = 0; i < 0x10000; ++i) {
    assert (uchar_isBinDigit (i) == (i == '0' || i == '1'));
  }

  printOk ();
  ___BTPOP;
}

static void testIsDigit (void) {
  ___BTPUSH;
  printTest ("uchar_isDigit");

  for (int i = 0; i < 0x10000; ++i) {
    assert (uchar_isDigit (i) == (i >= '0' && i <= '9'));
  }

  printOk ();
  ___BTPOP;
}

static void testIsHexDigit (void) {
  ___BTPUSH;
  printTest ("uchar_isHexDigit");

  for (int i = 0; i < 0x10000; ++i) {
    assert (uchar_isHexDigit (i) == ((i >= '0' && i <= '9') ||  (i >= 'A' && i <= 'F') || (i >= 'a' && i <= 'f')));
  }

  printOk ();
  ___BTPOP;
}

static void testIsOctDigit (void) {
  ___BTPUSH;
  printTest ("uchar_isOctDigit");

  for (int i = 0; i < 0x10000; ++i) {
    assert (uchar_isOctDigit (i) == (i >= '0' && i <= '7'));
  }

  printOk ();
  ___BTPOP;
}

static void testToUtf8 (void) {
  ___BTPUSH;
  printTest ("uchar_toUtf8");

  for (int i = 0; i < 0x10000; ++i) {
    assert (uchar_toUtf8 (i) == toUtf8 (i));
  }

  printOk ();
  ___BTPOP;
}

const struct ucharTestsStruct ucharTests = {
  testHexDigitValue,
  testIsBinDigit,
  testIsDigit,
  testIsHexDigit,
  testIsOctDigit,
  testToUtf8
};

static int toUtf8 (uchar_t cr) {
  int result;
  int sig = int_bsr (cr);
  if (sig < 8) {
    result = cr;
  } else if (sig < 12) {
    result = (cr & 0x3F | 0x80) << 8;
    result |= cr >> 6 | 0xC0;
  } else {
    result = (cr & 0x3F | 0x80) << 16;
    result |= (cr >> 6 & 0x3F | 0x80) << 8;
    result |= cr >> 12 | 0xE0;
  }
  return result;
}
