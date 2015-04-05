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

#include "defaultVideoMem.h"

REFL_IMPL (defaultVideoMem_t, videoMem_t)

defaultVideoMem_t::defaultVideoMem_t (void) : videoMem_t (NULL, NULL) {
}

uint8_t defaultVideoMem_t::readByte (int index) {
  return 0xFF;
}

void defaultVideoMem_t::writeByte (int index, uint8_t value) {
}
