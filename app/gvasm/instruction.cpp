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
#include "instruction.h"
#include "instructionCodes.h"

I_REFL_IMPL (instruction_t, byteBlock_t)

instruction_t::instruction_t (xValue_t* csOffset, int id, operand_t* operand1, operand_t* operand2) : byteBlock_t (csOffset),
  m_group (id & MASK_IID_GRP), m_operationCode (id >> WIDTH_IID_GRP), m_operand1 (operand1), m_operand2 (operand2) {
}

instruction_t::~instruction_t (void) {
  ___CBTPUSH;

  if (m_operand1 != NULL) {
    m_operand1->dump ();
    if (m_operand2 != NULL) {
      m_operand2->dump ();
    }
  }

  ___CBTPOP;
}

int instruction_t::group (void) {
  return m_group;
}

operand_t* instruction_t::operand1 (void) {
  return m_operand1;
}

operand_t* instruction_t::operand2 (void) {
  return m_operand2;
}

int instruction_t::operationCode (void) {
  return m_operationCode;
}

byteBlock_t::type_t instruction_t::type (void) {
  return TYPE_INSTRUCTION;
}
