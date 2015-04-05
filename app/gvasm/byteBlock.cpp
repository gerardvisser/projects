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

#include <limits.h>
#include <lombok/base/errors.h>
#include "byteBlock.h"

I_REFL_IMPL (byteBlock_t, object_t)

byteBlock_t::byteBlock_t (xValue_t* csOffset) : m_location (new xRelValue_t (csOffset, 0x9000000000000000)) {
}

byteBlock_t::~byteBlock_t (void) {
  ___CBTPUSH;

  m_location->dump ();

  ___CBTPOP;
}

bool byteBlock_t::hasLocation (void) {
  ___CBTPUSH;

  bool result = m_location->get () >= INT_MIN;

  ___CBTPOP;
  return result;
}

int byteBlock_t::location (void) {
  ___CBTPUSH;

  int result = m_location->get ();

  ___CBTPOP;
  return result;
}
