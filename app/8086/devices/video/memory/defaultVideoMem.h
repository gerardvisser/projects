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

#ifndef ___DEBUG_DEVICES_VIDEO_MEMORY__DEFAULT_VIDEO_MEM_INCLUDED
#define ___DEBUG_DEVICES_VIDEO_MEMORY__DEFAULT_VIDEO_MEM_INCLUDED

#include "videoMem.h"

class defaultVideoMem_t : public videoMem_t {
public:
  defaultVideoMem_t (void);

  virtual uint8_t readByte (int index);
  virtual void writeByte (int index, uint8_t value);

  REFL_DECL
};

#endif
