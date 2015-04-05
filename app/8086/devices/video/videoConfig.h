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

#ifndef ___DEBUG_DEVICES_VIDEO__VIDEO_CONFIG_INCLUDED
#define ___DEBUG_DEVICES_VIDEO__VIDEO_CONFIG_INCLUDED

#include <stdint.h>
#include "addressingMode.h"
#include "memoryMapMode.h"
#include "serializationMode.h"

typedef struct {
  serializationMode_t serializationMode;
  addressingMode_t addressingMode;
  memoryMapMode_t memoryMapMode;
  uint16_t horizontalDisplayEnd;
  uint16_t verticalDisplayEnd;
  uint8_t characterWidth;
  bool cgaScanLineMode;
  bool graphicsMode;
  bool widePixels;
} videoConfig_t;

#endif
