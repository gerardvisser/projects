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

#ifndef ___DEBUG_DEVICES_VIDEO__DAC_INCLUDED
#define ___DEBUG_DEVICES_VIDEO__DAC_INCLUDED

#include <stdint.h>
#include <lombok/base/class.h>
#include "colour.h"

class dac_t : public object_t {
private:
  uint8_t* m_palette;

public:
  dac_t (void);
  virtual ~dac_t (void);

  virtual void getColour (colour_t* colour, int index) const;
  virtual void getRGB (uint8_t* rgb, int index) const;
  virtual void setRGB (int index, uint8_t* rgb);

  REFL_DECL
};

#endif
