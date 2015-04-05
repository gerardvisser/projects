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
#include "orderedLabel.h"

REFL_IMPL (orderedLabel_t, object_t)

orderedLabel_t::orderedLabel_t (int index, xValue_t* value) : m_value (value), m_index (index) {
  ___CBTPUSH;

  m_value->reserve ();

  ___CBTPOP;
}

orderedLabel_t::~orderedLabel_t (void) {
  ___CBTPUSH;

  m_value->dump ();

  ___CBTPOP;
}

int orderedLabel_t::index (void) {
  return m_index;
}

xValue_t* orderedLabel_t::value (void) {
  return m_value;
}
