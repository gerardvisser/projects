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

#include <stddef.h>
#include <lombok/base/errors.h>
#include "xBranch.h"

I_REFL_IMPL (xBranch_t, xTree_t)

xBranch_t::xBranch_t (int precedence) : m_precedence (precedence), m_leftChild (NULL), m_rightChild (NULL) {
  ___CBTPUSH;
  ___CBTPOP;
}

xBranch_t::~xBranch_t (void) {
  ___CBTPUSH;

  if (m_leftChild != NULL) {
    m_leftChild->dump ();
  }
  if (m_rightChild != NULL) {
    m_rightChild->dump ();
  }

  ___CBTPOP;
}

bool xBranch_t::hasConstantValue (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (m_leftChild == NULL) {
    errors_printMessageAndExit ("%s instance has no left child", getClass ()->name);
  } else if (m_rightChild == NULL) {
    errors_printMessageAndExit ("%s instance has no right child", getClass ()->name);
  }
#endif

  bool result = m_leftChild->hasConstantValue () & m_rightChild->hasConstantValue ();

  ___CBTPOP;
  return result;
}

xTree_t* xBranch_t::leftChild (void) {
  return m_leftChild;
}

int xBranch_t::precedence (void) {
  return m_precedence;
}

xTree_t* xBranch_t::rightChild (void) {
  return m_rightChild;
}

void xBranch_t::setLeftChild (xTree_t* tree) {
  ___CBTPUSH;

  if (tree != NULL) {
    tree->wm_parent = this;
  }
  m_leftChild = tree;

  ___CBTPOP;
}

void xBranch_t::setRightChild (xTree_t* tree) {
  ___CBTPUSH;

  if (tree != NULL) {
    tree->wm_parent = this;
  }
  m_rightChild = tree;

  ___CBTPOP;
}
