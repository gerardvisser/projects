/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2014 Gerard Visser.

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

#include "fixedSizeInstruction.h"

REFL_IMPL (fixedSizeInstruction_t, instruction_t)

fixedSizeInstruction_t::fixedSizeInstruction_t (xValue_t* csOffset, int id, int size, operand_t* operand1, operand_t* operand2) :
  instruction_t (csOffset, id, operand1, operand2), m_size (size) {
}

bool fixedSizeInstruction_t::hasModrm (void) {
  return false;
}

int fixedSizeInstruction_t::size (void) {
  return m_size;
}
