/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

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

#include <stdio.h>
#include <lombok/base/uchar.h>

int uchar_hexDigitValue (uchar_t cr) {
  cr -= 0x30;
  if (cr > 9) {
    cr -= 7;
    if (cr > 0xF) {
      cr -= 0x20;
    }
  }
  return cr;
}

bool uchar_isBinDigit (uchar_t cr) {
  return cr == 0x30 || cr == 0x31;
}

bool uchar_isDigit (uchar_t cr) {
  return cr > 0x2F && cr < 0x3A;
}

bool uchar_isHexDigit (uchar_t cr) {
  return (cr > 0x2F && cr < 0x3A) || (cr > 0x40 && cr < 0x47) || (cr > 0x60 && cr < 0x67);
}

bool uchar_isOctDigit (uchar_t cr) {
  return cr > 0x2F && cr < 0x38;
}

void uchar_put (uchar_t cr) {
  int val = uchar_toUtf8 (cr);
  while (val > 0) {
    putchar (val & 0xFF);
    val >>= 8;
  }
}

int uchar_toUtf8 (uchar_t cr) {
  int result;

  if (cr < 0x80) {
    result = cr;
  } else if (cr < 0x800) {
    result = (cr & 0x3F | 0x80) << 8;
    result |= cr >> 6 | 0xC0;
  } else {
    result = (cr & 0x3F | 0x80) << 16;
    result |= (cr >> 6 & 0x3F | 0x80) << 8;
    result |= cr >> 12 | 0xE0;
  }
  return result;
}
