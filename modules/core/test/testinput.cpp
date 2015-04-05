/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2012, 2014 Gerard Visser.

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

#include <lombok/base/uchar.h>
#include "testinput.h"

static int bytes;
static int nextCharIndex;

int testinput_nextByte (void) {
  int result;
  if (bytes == 0) {
    int cr = testinput_nextChar ();
    if (cr == -1) {
      return -1;
    }
    bytes = uchar_toUtf8 (cr);
  }
  result = bytes & 0xFF;
  bytes >>= 8;
  return result;
}

int testinput_nextChar (void) {
  int result;
  int r = nextCharIndex & 0xFF;
  switch (nextCharIndex >> 8) {
  case 0: result = r; break;
  case 1: result = r + 0x780; break;
  case 2: result = r + 0x2000; break;
  case 3: result = r + 0xFF00; break;
  default: result = -1; break;
  }
  ++nextCharIndex;
  return result;
}

void testinput_reset (void) {
  nextCharIndex = 0;
  bytes = 0;
}
