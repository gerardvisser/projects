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
#include "chain2VideoMem.h"

REFL_IMPL (chain2VideoMem_t, videoMem_t)

static const uint32_t planeMask[] = {0x00FF00FF, 0xFF00FF00};

chain2VideoMem_t::chain2VideoMem_t (uint32_t* buffer, const graphicsRegisters_t* graphicsRegisters) : videoMem_t (buffer, graphicsRegisters) {
}

int chain2VideoMem_t::getBufferIndex (int index) {
  return index >> 1;
}

int chain2VideoMem_t::getPlaneToRead (int index) {
  ___CBTPUSH;

  int result = videoMem_t::getPlaneToRead (index) & 0xFE | index & 1;

  ___CBTPOP;
  return result;
}

uint32_t chain2VideoMem_t::getPlaneWriteEnableMask (int index) {
  ___CBTPUSH;

  uint32_t result = videoMem_t::getPlaneWriteEnableMask (index) & planeMask[index & 1];

  ___CBTPOP;
  return result;
}
