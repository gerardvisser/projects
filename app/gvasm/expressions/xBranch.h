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

#ifndef ___GVASM__EXPRESSIONS__X_BRANCH_INCLUDED
#define ___GVASM__EXPRESSIONS__X_BRANCH_INCLUDED

#include "xTree.h"

class xBranch_t : public xTree_t {
private:
  xTree_t* m_leftChild;
  xTree_t* m_rightChild;
  const int m_precedence;

public:
  xBranch_t (int precedence);
  virtual ~xBranch_t (void);

  virtual bool hasConstantValue (void);
  virtual xTree_t* leftChild (void);
  virtual int precedence (void);
  virtual xTree_t* rightChild (void);
  virtual void setLeftChild (xTree_t* tree);
  virtual void setRightChild (xTree_t* tree);

  I_REFL_DECL
};

#endif
