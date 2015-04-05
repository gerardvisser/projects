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

#include "register.h"

REFL_IMPL (ops::register_t, operand_t)

using namespace ops;

register_t::register_t (int regId, operandSize_t size) : operand_t (size), m_regId (regId) {
}

operandType_t register_t::type (void) const {
  return OPERAND_TYPE_REGISTER;
}

int register_t::value (void) const {
  return m_regId;
}
