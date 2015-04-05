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

#ifndef ___DEBUG_DEVICES_VIDEO__GRAPHICS_REGISTERS_INCLUDED
#define ___DEBUG_DEVICES_VIDEO__GRAPHICS_REGISTERS_INCLUDED

#include <stdint.h>

typedef struct {
  uint8_t setReset;
  uint8_t enableSetReset;
  uint8_t colourCompare;
  uint8_t logicalOperation;
  uint8_t rotateCount;
  uint8_t readPlaneSelect;
  uint8_t readMode;
  uint8_t writeMode;
  uint8_t colourDontCare;
  uint8_t bitMask;
  uint8_t writePlaneEnable;
} graphicsRegisters_t;

#endif