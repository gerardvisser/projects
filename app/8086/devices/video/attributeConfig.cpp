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
#include "attributeConfig.h"

REFL_IMPL (attributeConfig_t, object_t)

attributeConfig_t::attributeConfig_t (void) {
  ___CBTPUSH;

  for (int i = 0; i < 16; ++i) {
    m_paletteRegs[i] = i;
  }
  m_bitsP5P4Select = false;
  m_colourSelect = 0;

  ___CBTPOP;
}

int attributeConfig_t::getDacIndex (int index) const {
  ___CBTPUSH;

  int result;
  index &= 0xF;
  if (m_bitsP5P4Select) {
    result = m_colourSelect << 4 | m_paletteRegs[index] & 0xF;
  } else {
    result = m_colourSelect << 4 & 0xC0 | m_paletteRegs[index] & 0x3F;
  }

  ___CBTPOP;
  return result;
}
