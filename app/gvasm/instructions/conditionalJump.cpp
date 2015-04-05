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

#include "conditionalJump.h"

REFL_IMPL (conditionalJump_t, fixedSizeInstruction_t)

conditionalJump_t::conditionalJump_t (xValue_t* csOffset, int id, operand_t* operand, int line, int column) : fixedSizeInstruction_t (csOffset, id, 2, operand),
  m_line (line), m_column (column) {
}

int conditionalJump_t::column (void) {
  return m_column;
}

int conditionalJump_t::line (void) {
  return m_line;
}
