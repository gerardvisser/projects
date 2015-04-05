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

#ifndef ___DEBUG_DEVICES_VIDEO__ATTRIBUTE_CONFIG_INCLUDED
#define ___DEBUG_DEVICES_VIDEO__ATTRIBUTE_CONFIG_INCLUDED

#include <stdint.h>
#include <lombok/base/class.h>

class attributeConfig_t : public object_t {
public:
  uint8_t m_paletteRegs[16];
  uint8_t m_colourSelect;
  bool m_bitsP5P4Select;

  attributeConfig_t (void);

  virtual int getDacIndex (int index) const;

  REFL_DECL
};

#endif
