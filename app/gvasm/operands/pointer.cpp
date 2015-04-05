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
#include "pointer.h"

REFL_IMPL (ops::pointer_t, operand_t)

using namespace ops;

pointer_t::pointer_t (int modrmValue, xTree_t* displacement, operandSize_t size) : operand_t (size), m_modrmValue (modrmValue) {
  ___CBTPUSH;

  if (displacement == NULL) {
    m_displacement.integer = 0;
    m_hasTreeDisplacement = false;
  } else if (displacement->hasConstantValue ()) {
    m_displacement.integer = displacement->value ();
    m_hasTreeDisplacement = false;
    displacement->dump ();
  } else {
    m_displacement.tree = displacement;
    m_hasTreeDisplacement = true;
  }

  ___CBTPOP;
}

pointer_t::~pointer_t (void) {
  ___CBTPUSH;

  if (m_hasTreeDisplacement) {
    m_displacement.tree->dump ();
  }

  ___CBTPOP;
}

int pointer_t::displacement (void) const {
  ___CBTPUSH;

  int result = m_hasTreeDisplacement ? m_displacement.tree->value () : m_displacement.integer;

  ___CBTPOP;
  return result;
}

operandType_t pointer_t::type (void) const {
  return OPERAND_TYPE_POINTER;
}

int pointer_t::value (void) const {
  ___CBTPUSH;

  int result = m_modrmValue;
  if (result >= 0x80) {
    int disp = displacement ();
    if (disp == 0) {
      result -= result == 0x86 ? 0x40 : 0x80;
    } else if (disp > -0x81 && disp < 0x80) {
      result -= 0x40;
    }
  }

  ___CBTPOP;
  return result;
}
