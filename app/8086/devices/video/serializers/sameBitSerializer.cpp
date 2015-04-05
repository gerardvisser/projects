/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2015 Gerard Visser.

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

#include <lombok/base/errors.h>
#include "sameBitSerializer.h"

REFL_IMPL (sameBitSerializer_t, serializer_t)

sameBitSerializer_t::sameBitSerializer_t (const uint32_t* buffer) : serializer_t (buffer) {
}

uint16_t sameBitSerializer_t::read (int index) {
  ___CBTPUSH;

  const int i = index << 1 & 0xFFFFFFFC;
  const int s = (index & 1) != 0 ? 0 : 4;
  int result = wm_buffer[i + 3] >> s & 0xF;
  result <<= 4;
  result |= wm_buffer[i + 2] >> s & 0xF;
  result <<= 4;
  result |= wm_buffer[i + 1] >> s & 0xF;
  result <<= 4;
  result |= wm_buffer[i] >> s & 0xF;

  ___CBTPOP;
  return result;
}
