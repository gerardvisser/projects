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
#include "xRelValue.h"

REFL_IMPL (xRelValue_t, xValue_t)

xRelValue_t::xRelValue_t (xValue_t* base, int64_t value) : xValue_t (value), m_base (base) {
  ___CBTPUSH;

  m_base->reserve ();

  ___CBTPOP;
}

xRelValue_t::~xRelValue_t (void) {
  ___CBTPUSH;

  m_base->dump ();

  ___CBTPOP;
}

int64_t xRelValue_t::get (void) {
  ___CBTPUSH;

  int64_t result = xValue_t::get ();
  int64_t base = m_base->get ();
  if (result >= base) {
    result -= base;
  }

  ___CBTPOP;
  return result;
}
