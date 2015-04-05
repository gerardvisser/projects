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

#include <lombok/base/errors.h>
#include "modrmImmInstruction.h"
#include "../instructionCodes.h"

REFL_IMPL (modrmImmInstruction_t, modrmInstruction_t)

modrmImmInstruction_t::modrmImmInstruction_t (xValue_t* csOffset, int id, bool fixedSize, operand_t* operand1, operand_t* operand2) :
  modrmInstruction_t (csOffset, id, fixedSize, operand1, operand2) {
}

int modrmImmInstruction_t::calculatedSize (void) {
  ___CBTPUSH;

  int result = modrmInstruction_t::calculatedSize ();
  if (group () == GRP_ADD && operand2 ()->size () == OPERAND_SIZE_16_BITS) {
    int immValue = operand2 ()->value () & 0xFFFF;
    result -= immValue < 0x80 || immValue > 0xFF7F;
  }

  ___CBTPOP;
  return result;
}

int modrmImmInstruction_t::size (void) {
  ___CBTPUSH;

  int result = modrmInstruction_t::size () + operand2 ()->size ();

  ___CBTPOP;
  return result;
}
