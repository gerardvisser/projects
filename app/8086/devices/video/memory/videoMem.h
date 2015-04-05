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

#ifndef ___DEBUG_DEVICES_VIDEO_MEMORY__VIDEO_MEM_INCLUDED
#define ___DEBUG_DEVICES_VIDEO_MEMORY__VIDEO_MEM_INCLUDED

#include <stdint.h>
#include <lombok/base/class.h>
#include "../graphicsRegisters.h"

class videoMem_t : public object_t {
private:
  const graphicsRegisters_t* const wm_graphicsRegisters;
  uint32_t* const wm_buffer;
  uint32_t m_latch;

public:
  videoMem_t (uint32_t* buffer, const graphicsRegisters_t* graphicsRegisters);

  virtual uint8_t readByte (int index);
  virtual uint16_t readWord (int index);
  virtual void writeByte (int index, uint8_t value);
  virtual void writeWord (int index, uint16_t value);

protected:
  virtual int getBufferIndex (int index);
  virtual int getPlaneToRead (int index);
  virtual uint32_t getPlaneWriteEnableMask (int index);

private:
  uint32_t applyLogOp (uint32_t planes);
  uint32_t applyMask (uint32_t planes, uint8_t mask);
  uint32_t replicate (uint8_t value);
  uint8_t ror (uint8_t value);
  uint32_t setResetPlanes (uint32_t planes, int enable);

  REFL_DECL
};

#endif
