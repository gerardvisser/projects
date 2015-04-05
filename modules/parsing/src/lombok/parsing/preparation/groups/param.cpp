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
#include <lombok/base/objectReleasePool.h>
#include <lombok/util/arrayList.h>
#include "groupIDs.h"
#include "param.h"

REFL_IMPL (pars::param_t, pars::block_t)

using namespace pars;

static arrayList_t* params = NULL;

param_t::param_t (int ordinal) : block_t (GID_PARAM, 0, 0), ordinal (ordinal) {
}

param_t* param_t::get (int ordinal) {
  ___CBTPUSH;

  if (params == NULL) {
    params = new arrayList_t ();
    objectReleasePool_t::instance ()->add (params);
  }
  if (ordinal >= params->length ()) {
    for (int i = params->length (); i < ordinal + 1; ++i) {
      params->append (new param_t (i));
    }
  }
  param_t* result = (param_t*) params->get (ordinal);
  result->reserve ();

  ___CBTPOP;
  return result;
}
