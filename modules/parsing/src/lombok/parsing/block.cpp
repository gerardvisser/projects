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
#include <lombok/parsing/block.h>

I_REFL_IMPL (pars::block_t, object_t)

using namespace pars;

block_t::block_t (int id, int contextCode, int errorCode) : _id (id), _contextCode (contextCode), _errorCode (errorCode) {
}

int block_t::contextCode (void) {
  return _contextCode;
}

int block_t::errorCode (void) {
  return _errorCode;
}

int block_t::id (void) {
  return _id;
}
