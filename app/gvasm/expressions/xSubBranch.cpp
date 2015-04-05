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
#include "precedence.h"
#include "xSubBranch.h"

REFL_IMPL (xSubBranch_t, xBranch_t)

xSubBranch_t::xSubBranch_t (void) : xBranch_t (SUB_PRECEDENCE) {
}

int64_t xSubBranch_t::value (void) {
  ___CBTPUSH;

  int64_t result = leftChild ()->value () - rightChild ()->value ();

  ___CBTPOP;
  return result;
}
