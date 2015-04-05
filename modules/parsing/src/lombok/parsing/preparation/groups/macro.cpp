/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013 Gerard Visser.

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
#include "groupIDs.h"
#include "macro.h"

REFL_IMPL (pars::macro_t, pars::block_t)

using namespace pars;

macro_t::macro_t (string_t* name, list_t* blocks, int argCount, bool hasArgs, bool hasVarArgs) :
  block_t (GID_DEFINE, 0, 0), m_name (name), m_blocks (blocks), m_argCount (argCount), m_hasArgs (hasArgs + hasVarArgs) {
}

macro_t::~macro_t (void) {
  ___CBTPUSH;

  m_name->dump ();
  if (m_blocks != NULL) {
    m_blocks->dump ();
  }

  ___CBTPOP;
}

int macro_t::argCount (void) {
  return m_argCount;
}

bool macro_t::hasArgs (void) {
  return m_hasArgs > 0;
}

bool macro_t::hasVarArgs (void) {
  return m_hasArgs > 1;
}

string_t* macro_t::name (void) {
  return m_name;
}

list_t* macro_t::replacement (void) {
  return m_blocks;
}
