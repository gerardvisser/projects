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
#include "include.h"

REFL_IMPL (pars::include_t, pars::block_t)

using namespace pars;

include_t::include_t (filename_t* filename) : block_t (GID_INCLUDE, 0, 0), m_filename (filename) {
}

include_t::~include_t (void) {
  ___CBTPUSH;

  m_filename->dump ();

  ___CBTPOP;
}

filename_t* include_t::filename (void) {
  return m_filename;
}
