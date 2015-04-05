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

#include "operand.h"

I_REFL_IMPL (operand_t, object_t)

operand_t::operand_t (operandSize_t size) : m_size (size) {
}

int operand_t::displacement (void) const {
  return 0;
}

bool operand_t::isAccumulator (void) const {
  return type () == OPERAND_TYPE_REGISTER && value () == 0;
}

bool operand_t::isImmediate (void) const {
  return type () == OPERAND_TYPE_IMMEDIATE;
}

bool operand_t::isPointer (void) const {
  return type () == OPERAND_TYPE_POINTER;
}

bool operand_t::isRegister (void) const {
  return type () == OPERAND_TYPE_REGISTER;
}

bool operand_t::isSegRegister (void) const {
  return type () == OPERAND_TYPE_SEGREGISTER;
}

bool operand_t::isWide (void) const {
  return m_size == OPERAND_SIZE_16_BITS;
}

operandSize_t operand_t::size (void) const {
  return m_size;
}
