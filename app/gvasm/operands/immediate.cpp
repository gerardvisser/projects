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
#include "immediate.h"

REFL_IMPL (ops::immediate_t, operand_t)

using namespace ops;

immediate_t::immediate_t (xTree_t* tree) {
  ___CBTPUSH;

  m_hasTreeValue = !tree->hasConstantValue ();
  if (m_hasTreeValue) {
    m_value.tree = tree;
  } else {
    m_value.integer = tree->value ();
    tree->dump ();
  }

  ___CBTPOP;
}

immediate_t::~immediate_t (void) {
  ___CBTPUSH;

  if (m_hasTreeValue) {
    m_value.tree->dump ();
  }

  ___CBTPOP;
}

operandType_t immediate_t::type (void) const {
  return OPERAND_TYPE_IMMEDIATE;
}

int immediate_t::value (void) const {
  ___CBTPUSH;

  int result = m_hasTreeValue ? m_value.tree->value () : m_value.integer;

  ___CBTPOP;
  return result;
}
