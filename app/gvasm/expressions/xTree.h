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

#ifndef ___GVASM__EXPRESSIONS__X_TREE_INCLUDED
#define ___GVASM__EXPRESSIONS__X_TREE_INCLUDED

#include <stdint.h>
#include <lombok/base/class.h>

class xBranch_t;

class xTree_t : public object_t {
private:
  xBranch_t* wm_parent;

public:
  xTree_t (void);
  virtual ~xTree_t (void);

  virtual xBranch_t* addBranch (int tokenId);
  virtual bool hasConstantValue (void) = 0;
  virtual bool isRoot (void);
  virtual xBranch_t* parent (void);
  virtual int64_t value (void) = 0;

  I_REFL_DECL

  friend class xBranch_t;
};

#endif
