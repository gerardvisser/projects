/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2014, 2015 Gerard Visser.

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

#ifndef ___DEBUG_MEMORY_INCLUDED
#define ___DEBUG_MEMORY_INCLUDED

#include "devices/video/memory/videoMem.h"

class memory_t : public object_t {
private:
  videoMem_t* m_videoMem;
  uint8_t* m_lbuf;
  uint8_t* m_hbuf;

public:
  memory_t (void);
  virtual ~memory_t (void);

  virtual int read (int index, bool wide = false) const;
  virtual int readByte (int index) const;
  virtual int readByteAsWord (int index) const;
  virtual void readBytes (uint8_t* dst, int index, uint16_t count) const;
  virtual int readWord (int index) const;
  virtual void replaceVideoMem (videoMem_t* videoMem);
  virtual void write (int index, int val, bool wide = false);
  virtual void writeByte (int index, int val);
  virtual void writeBytes (int index, const uint8_t* src, uint16_t count);
  virtual void writeBytes (int index, int val, uint16_t count);
  virtual void writeWord (int index, int val);
  virtual void writeWords (int index, int val, uint16_t count);

  REFL_DECL
};

#endif
