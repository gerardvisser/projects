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
#include "videoMem.h"

REFL_IMPL (videoMem_t, object_t)

/*
  Layout of the buffer:
  Each location contains a 32 bit value, each byte of which represents a bit plane:
  plane 3:   plane 2:   plane 1:   plane 0:
  .... ....  .... ....  .... ....  .... ....  */

static const uint32_t colcmp[] = {
  0x00000000, 0x000000FF, 0x0000FF00, 0x0000FFFF, 0x00FF0000, 0x00FF00FF, 0x00FFFF00, 0x00FFFFFF,
  0xFF000000, 0xFF0000FF, 0xFF00FF00, 0xFF00FFFF, 0xFFFF0000, 0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF
};

videoMem_t::videoMem_t (uint32_t* buffer, const graphicsRegisters_t* graphicsRegisters) : wm_buffer (buffer), wm_graphicsRegisters (graphicsRegisters) {
  ___CBTPUSH;
  ___CBTPOP;
}

uint32_t videoMem_t::applyLogOp (uint32_t planes) {
  ___CBTPUSH;

  switch (wm_graphicsRegisters->logicalOperation) {
  case 1: planes &= m_latch; break;
  case 2: planes |= m_latch; break;
  case 3: planes ^= m_latch;
  }

  ___CBTPOP;
  return planes;
}

uint32_t videoMem_t::applyMask (uint32_t planes, uint8_t mask) {
  ___CBTPUSH;

  uint32_t mask32 = replicate (mask);
  planes &= mask32;
  planes |= m_latch & (mask32 ^ -1);

  ___CBTPOP;
  return planes;
}

int videoMem_t::getBufferIndex (int index) {
  return index;
}

int videoMem_t::getPlaneToRead (int index) {
  ___CBTPUSH;

  int result = wm_graphicsRegisters->readPlaneSelect;

  ___CBTPOP;
  return result;
}

uint32_t videoMem_t::getPlaneWriteEnableMask (int index) {
  ___CBTPUSH;

  uint32_t result = colcmp[wm_graphicsRegisters->writePlaneEnable];

  ___CBTPOP;
  return result;
}

uint8_t videoMem_t::readByte (int index) {
  ___CBTPUSH;

  uint8_t result;
  m_latch = wm_buffer[getBufferIndex (index)];
  if (wm_graphicsRegisters->readMode != 0) {
    uint32_t tmp = m_latch ^ colcmp[wm_graphicsRegisters->colourCompare] ^ -1;
    tmp |= colcmp[wm_graphicsRegisters->colourDontCare] ^ -1;
    result = tmp >> 24;
    result &= tmp >> 16;
    result &= tmp >> 8;
    result &= tmp;
  } else {
    result = m_latch >> 8 * getPlaneToRead (index);
  }

  ___CBTPOP;
  return result;
}

uint16_t videoMem_t::readWord (int index) {
  ___CBTPUSH;

  uint16_t result = readByte (index + 1) << 8;
  result |= readByte (index);

  ___CBTPOP;
  return result;
}

uint32_t videoMem_t::replicate (uint8_t value) {
  uint32_t result = value;
  result <<= 8;
  result |= value;
  result <<= 8;
  result |= value;
  result <<= 8;
  result |= value;
  return result;
}

uint8_t videoMem_t::ror (uint8_t value) {
  ___CBTPUSH;

  int result = value;
  result <<= 8;
  result |= value;
  result >>= wm_graphicsRegisters->rotateCount;

  ___CBTPOP;
  return result;
}

uint32_t videoMem_t::setResetPlanes (uint32_t planes, int enable) {
  ___CBTPUSH;

  planes &= colcmp[enable] ^ -1;
  planes |= colcmp[wm_graphicsRegisters->setReset & enable];

  ___CBTPOP;
  return planes;
}

void videoMem_t::writeByte (int index, uint8_t value) {
  ___CBTPUSH;

  uint32_t planes;
  switch (wm_graphicsRegisters->writeMode) {
  case 0:
    value = ror (value);
    planes = replicate (value);
    planes = setResetPlanes (planes, wm_graphicsRegisters->enableSetReset);
    planes = applyLogOp (planes);
    planes = applyMask (planes, wm_graphicsRegisters->bitMask);
    break;

  case 1:
    planes = m_latch;
    break;

  case 2:
    planes = colcmp[value & 15];
    planes = applyLogOp (planes);
    planes = applyMask (planes, wm_graphicsRegisters->bitMask);
    break;

  case 3:
    value = ror (value);
    value &= wm_graphicsRegisters->bitMask;
    planes = setResetPlanes (0, 15);
    planes = applyMask (planes, value);
    break;
  }
  int bufidx = getBufferIndex (index);
  uint32_t mask = getPlaneWriteEnableMask (index);
  wm_buffer[bufidx] &= mask ^ -1;
  wm_buffer[bufidx] |= planes & mask;

  ___CBTPOP;
}

void videoMem_t::writeWord (int index, uint16_t value) {
  ___CBTPUSH;

  writeByte (index, value);
  writeByte (index + 1, value >> 8);

  ___CBTPOP;
}
