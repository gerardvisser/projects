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

#include <lombok/testing/testUtils.h>
#include "intTests.h"

static void testBsf (void) {
  ___BTPUSH;
  printTest ("int_bsf");

  assert (int_bsf (0x00) == 0);
  assert (int_bsf (0x01) == 0);
  assert (int_bsf (0x02) == 1);
  assert (int_bsf (0x03) == 0);
  assert (int_bsf (0x04) == 2);
  assert (int_bsf (0x05) == 0);
  assert (int_bsf (0x06) == 1);
  assert (int_bsf (0x07) == 0);
  assert (int_bsf (0x08) == 3);
  assert (int_bsf (0x09) == 0);
  assert (int_bsf (0x0A) == 1);
  assert (int_bsf (0x0B) == 0);
  assert (int_bsf (0x0C) == 2);
  assert (int_bsf (0x0D) == 0);
  assert (int_bsf (0x0E) == 1);
  assert (int_bsf (0x0F) == 0);
  assert (int_bsf (0x10) == 4);
  assert (int_bsf (0x80000000) == 31);
  assert (int_bsf (0x90000000) == 28);
  assert (int_bsf (0xA0000000) == 29);
  assert (int_bsf (0xB0000000) == 28);
  assert (int_bsf (0xC0000000) == 30);
  assert (int_bsf (0xD0000000) == 28);
  assert (int_bsf (0xD8740000) == 18);
  assert (int_bsf (0xD8740002) == 1);
  assert (int_bsf (0xE0000000) == 29);
  assert (int_bsf (0xF0000000) == 28);
  assert (int_bsf (-8) == 3);
  assert (int_bsf (-6) == 1);
  assert (int_bsf (-4) == 2);
  assert (int_bsf (-2) == 1);
  assert (int_bsf (-1) == 0);

  printOk ();
  ___BTPOP;
}

static void testBsr (void) {
  ___BTPUSH;
  printTest ("int_bsr");

  assert (int_bsr (0x00) == 0);
  assert (int_bsr (0x01) == 1);
  assert (int_bsr (0x02) == 2);
  assert (int_bsr (0x03) == 2);
  assert (int_bsr (0x04) == 3);
  assert (int_bsr (0x05) == 3);
  assert (int_bsr (0x06) == 3);
  assert (int_bsr (0x07) == 3);
  assert (int_bsr (0x08) == 4);
  assert (int_bsr (0x09) == 4);
  assert (int_bsr (0x0A) == 4);
  assert (int_bsr (0x0B) == 4);
  assert (int_bsr (0x0C) == 4);
  assert (int_bsr (0x0D) == 4);
  assert (int_bsr (0x0E) == 4);
  assert (int_bsr (0x0F) == 4);
  assert (int_bsr (0x10) == 5);
  assert (int_bsr (0x20) == 6);
  assert (int_bsr (0xC3) == 8);
  assert (int_bsr (0xCD20) == 16);
  assert (int_bsr (0xE80000) == 24);
  assert (int_bsr (0x40003001) == 31);
  assert (int_bsr (0x80000000) == 32);
  assert (int_bsr (-2) == 32);
  assert (int_bsr (-1) == 32);

  printOk ();
  ___BTPOP;
}

const struct intTestsStruct intTests = {
  testBsf,
  testBsr
};
