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
#include "programme.h"

REFL_IMPL (programme_t, object_t)

programme_t::programme_t (arrayList_t* byteBlocks, int codeOffset) : m_byteBlocks (byteBlocks), m_csStart (codeOffset >> 4), m_ipStart (codeOffset & 0xF) {
  ___CBTPUSH;
  ___CBTPOP;
}

programme_t::~programme_t (void) {
  ___CBTPUSH;

  m_byteBlocks->dump ();

  ___CBTPOP;
}

arrayList_t* programme_t::byteBlocks (void) {
  return m_byteBlocks;
}

int programme_t::csStart (void) {
  return m_csStart;
}

int programme_t::ipStart (void) {
  return m_ipStart;
}
