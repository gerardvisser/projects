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
#include "modrmInstruction.h"

REFL_IMPL (modrmInstruction_t, instruction_t)

modrmInstruction_t::modrmInstruction_t (xValue_t* csOffset, int id, bool fixedSize, operand_t* operand1, operand_t* operand2) :
  instruction_t (csOffset, id, operand1, operand2), m_fixedSize (fixedSize) {
}

int modrmInstruction_t::calculatedSize (void) {
  ___CBTPUSH;

  int result = 2;
  operand_t* memOperand = NULL;
  if (operand1 ()->isPointer ()) {
    memOperand = operand1 ();
  } else if (operand2 () != NULL && operand2 ()->isPointer ()) {
    memOperand = operand2 ();
  }
  if (memOperand != NULL) {
    int modrm = memOperand->value ();
    if (modrm != 6) {
      result += modrm >> 6;
    } else {
      result += 2;
    }
  }

  ___CBTPOP;
  return result;
}

bool modrmInstruction_t::hasModrm (void) {
  return true;
}

int modrmInstruction_t::size (void) {
  ___CBTPUSH;

  int result;
  if (hasLocation () || m_fixedSize) {
    result = calculatedSize ();
  } else {
    result = 4;
  }

  ___CBTPOP;
  return result;
}
