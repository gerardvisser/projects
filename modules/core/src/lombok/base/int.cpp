/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2010 - 2012 Gerard Visser.

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

int int_bsf (int val) {
  int result = 0;
  if (val != 0) {
    int mask = 1;
    while ((val & mask) == 0) {
      mask <<= 1;
      ++result;
    }
  }
  return result;
}

int int_bsr (int val) {
  int mask, result;

  if (val != 0) {
    result = 32;
    mask = 0x80000000;
    while ((val & mask) == 0) {
      mask >>= 1;
      --result;
    }
  } else {
    result = 0;
  }
  return result;
}
