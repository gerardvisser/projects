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

#ifndef ___GVASM__OPERANDS__OPERAND_FACTORY_INCLUDED
#define ___GVASM__OPERANDS__OPERAND_FACTORY_INCLUDED

#include <lombok/parsing/token.h>
#include <lombok/util/arrayList.h>
#include <lombok/util/hashMap.h>
#include "../expressions/xConstLeaf.h"
#include "../data.h"
#include "../operand.h"

class operandFactory_t : public object_t {
private:
  arrayList_t* m_regTokensInExpression;
  arrayList_t* m_regLeavesInTree;
  xConstLeaf_t* m_zeroLeaf;
  hashMap_t* wm_labels;
  pars::token_t* wm_operandToken;
  int m_segmentOverrideIID;
  bool m_labelsUsed;

public:
  operandFactory_t (hashMap_t* labels);
  virtual ~operandFactory_t (void);

  virtual data_t* createData (listIterator_t* tokens, data_t::unitSize_t unitSize, xValue_t* csOffset);
  virtual operand_t* createOperand (listIterator_t* tokens);
  virtual bool labelsUsed (void);
  virtual pars::token_t* operandToken (void);
  virtual void reset (void);
  virtual int segmentOverrideIID (void);

private:
  xTree_t* createExpressionPart (listIterator_t* tokens);
  xTree_t* createExpressionTree (listIterator_t* tokens, bool parOpen = false);
  operand_t* createNumberOperand (listIterator_t* tokens);
  operand_t* createPointerOperand (listIterator_t* tokens, operandSize_t size = OPERAND_SIZE_UNKNOWN);

  REFL_DECL
};

#endif
