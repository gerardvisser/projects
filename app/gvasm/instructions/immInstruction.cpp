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
#include "immInstruction.h"

REFL_IMPL (immInstruction_t, instruction_t)

immInstruction_t::immInstruction_t (xValue_t* csOffset, int id, operand_t* operand1, operand_t* operand2) :
  instruction_t (csOffset, id, operand1, operand2) {
}

bool immInstruction_t::hasModrm (void) {
  return false;
}

int immInstruction_t::size (void) {
  ___CBTPUSH;

  int result = 1 + operand2 ()->size ();

  ___CBTPOP;
  return result;
}
