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
#include "../asmTokenIDs.h"
#include "xAddBranch.h"
#include "xDivBranch.h"
#include "xModBranch.h"
#include "xMulBranch.h"
#include "xSubBranch.h"

I_REFL_IMPL (xTree_t, object_t)

xTree_t::xTree_t (void) : wm_parent (NULL) {
}

xTree_t::~xTree_t (void) {
}

xBranch_t* xTree_t::addBranch (int tokenId) {
  ___CBTPUSH;

  xBranch_t* result;
  switch (tokenId) {
  case TID_S_PLUS: result = new xAddBranch_t (); break;
  case TID_S_MINUS: result = new xSubBranch_t (); break;
  case TID_S_ASTERISK: result = new xMulBranch_t (); break;
  case TID_S_SLASH: result = new xDivBranch_t (); break;
  case TID_S_PERCENT: result = new xModBranch_t (); break;
  default: /* Error */ break;
  }
  xTree_t* tree = this;
  while (!tree->isRoot () && tree->parent ()->precedence () >= result->precedence ()) {
    tree = tree->parent ();
  }
  if (!tree->isRoot ()) {
    tree->parent ()->setRightChild (result);
  }
  result->setLeftChild (tree);

  ___CBTPOP;
  return result;
}

bool xTree_t::isRoot (void) {
  return wm_parent == NULL;
}

xBranch_t* xTree_t::parent (void) {
  return wm_parent;
}
