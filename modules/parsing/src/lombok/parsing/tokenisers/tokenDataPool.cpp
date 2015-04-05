/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013, 2014 Gerard Visser.

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
#include <lombok/parsing/tokenisers/tokenDataPool.h>
#include <lombok/util/hashMap.h>

I_REFL_IMPL (pars::tokenDataPool_t, object_t)

using namespace pars;

tokenDataPool_t::tokenDataPool_t (void) {
  ___CBTPUSH;

  map = new hashMap_t (256);

  ___CBTPOP;
}

tokenDataPool_t::~tokenDataPool_t (void) {
  ___CBTPUSH;

  map->dump ();

  ___CBTPOP;
}

void tokenDataPool_t::add (const char* text, int id) {
  ___CBTPUSH;

  string_t* str = new string_t (text);
  map->set (str, new tokenData_t (str, id));

  ___CBTPOP;
}

tokenData_t* tokenDataPool_t::get (charSequence_t* text) {
  ___CBTPUSH;

  tokenData_t* result = (tokenData_t*) map->get (text);

  ___CBTPOP;
  return result;
}
