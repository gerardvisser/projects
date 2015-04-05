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
#include <lombok/parsing/contextCodes.h>
#include "../asmErrors.h"
#include "../asmTokenIDs.h"
#include "../expressions/precedence.h"
#include "../expressions/xLeaf.h"
#include "../expressions/xSubBranch.h"
#include "all.h"

REFL_IMPL (operandFactory_t, object_t)

using namespace pars;

static void checkForPrematureEndOfFile (token_t* token);
static void checkRegLocationInExpression (xConstLeaf_t* leaf, token_t* token);

operandFactory_t::operandFactory_t (hashMap_t* labels) {
  ___CBTPUSH;

  wm_labels = labels;
  m_zeroLeaf = new xConstLeaf_t (0);
  m_regLeavesInTree = new arrayList_t ();
  m_regTokensInExpression = new arrayList_t ();

  ___CBTPOP;
}

operandFactory_t::~operandFactory_t (void) {
  ___CBTPUSH;

  m_zeroLeaf->dump ();
  m_regLeavesInTree->dump ();
  m_regTokensInExpression->dump ();

  ___CBTPOP;
}

data_t* operandFactory_t::createData (listIterator_t* tokens, data_t::unitSize_t unitSize, xValue_t* csOffset) {
  ___CBTPUSH;

  int tokenId;
  data_t* data;
  arrayList_t* expressions = new arrayList_t ();
  const int alternativeComma = unitSize == data_t::UNIT_SIZE_8_BITS ? TID_P_DB : TID_P_DW;
  do {
    expressions->append (createExpressionTree (tokens));
    if (m_regLeavesInTree->length () != 0) {
      asmErrors::exit ("no registers allowed here", (token_t*) m_regLeavesInTree->get (0));
    }
    tokenId = ((token_t*) tokens->next ())->id ();
  } while (tokenId == TID_S_COMMA || tokenId == alternativeComma);

  tokens->previous ();
  if (expressions->length () == 1) {
    data = new data_t (csOffset, (xTree_t*) expressions->removeLast (), unitSize);
    expressions->dump ();
  } else {
    data = new data_t (csOffset, expressions, unitSize);
    expressions->trimToSize ();
  }

  ___CBTPOP;
  return data;
}

xTree_t* operandFactory_t::createExpressionPart (listIterator_t* tokens) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (!tokens->hasNext ()) {
    errors_printMessageAndExit ("No more tokens left");
  }
#endif

  xTree_t* result;
  token_t* token = (token_t*) tokens->next ();
  checkForPrematureEndOfFile (token);
  switch (token->id ()) {
  case TID_N_DEC:
  case TID_N_HEX:
  case TID_N_OCT:
  case BLTYPE_CHAR:
    result = new xConstLeaf_t (token->value ());
    break;

  case BLTYPE_PRIMARY: {
    xValue_t* value = (xValue_t*) wm_labels->get (token->text ());
    if (value == NULL) {
      asmErrors::exit ("no label named \x91%t\x92 was defined", token);
    }
    result = new xLeaf_t (value);
    m_labelsUsed = true;
  } break;

  case TID_P_BX:
    result = new xConstLeaf_t (0xD0);
    m_regLeavesInTree->append (result);
    m_regTokensInExpression->appendar (token);
    break;

  case TID_P_BP:
    result = new xConstLeaf_t (0x70);
    m_regLeavesInTree->append (result);
    m_regTokensInExpression->appendar (token);
    break;

  case TID_P_SI:
    result = new xConstLeaf_t (0x0D);
    m_regLeavesInTree->append (result);
    m_regTokensInExpression->appendar (token);
    break;

  case TID_P_DI:
    result = new xConstLeaf_t (0x07);
    m_regLeavesInTree->append (result);
    m_regTokensInExpression->appendar (token);
    break;

  case TID_S_PARENTHESIS_OPEN:
    result = createExpressionTree (tokens, true);
    break;

  default:
    asmErrors::exit ("stray \x91%t\x92 in programme", token);
  }

  ___CBTPOP;
  return result;
}

xTree_t* operandFactory_t::createExpressionTree (listIterator_t* tokens, bool parOpen) {
  ___CBTPUSH;

  xTree_t* tree = createExpressionPart (tokens);
  xTree_t* root = tree;
  token_t* token = (token_t*) tokens->next ();
  int tokenId = token->id ();
  while (tokenId == TID_S_PLUS || tokenId == TID_S_MINUS || tokenId == TID_S_ASTERISK || tokenId == TID_S_SLASH || tokenId == TID_S_PERCENT) {
    xBranch_t* branch = tree->addBranch (tokenId);
    if (branch->isRoot ()) {
      root = branch;
    }
    tree = createExpressionPart (tokens);
    branch->setRightChild (tree);
    token = (token_t*) tokens->next ();
    tokenId = token->id ();
  }
  if (parOpen) {
    checkForPrematureEndOfFile (token);
    if (tokenId != TID_S_PARENTHESIS_CLOSE) {
      asmErrors::exit ("expected \x91)\x92 before \x91%t\x92 token", token);
    }
  } else {
    tokens->previous ();
  }

  ___CBTPOP;
  return root;
}

operand_t* operandFactory_t::createNumberOperand (listIterator_t* tokens) {
  ___CBTPUSH;

  operand_t* operand;
  tokens->previous ();
  xTree_t* expression = createExpressionTree (tokens);
  if (m_regLeavesInTree->length () == 0) {
    operand = new ops::immediate_t (expression);
  } else {
    asmErrors::exit ("no registers allowed here", (token_t*) m_regLeavesInTree->get (0));
  }

  ___CBTPOP;
  return operand;
}

operand_t* operandFactory_t::createOperand (listIterator_t* tokens) {
  ___CBTPUSH;

  operand_t* operand;
  token_t* token = (token_t*) tokens->next ();
  int tokenId = token->id ();
  wm_operandToken = token;

  switch (tokenId & MASK_BLTYPE) {
  case BLTYPE_CHAR:
  case BLTYPE_NUMBER:
    operand = createNumberOperand (tokens);
    break;

  case BLTYPE_PRIMARY:
    switch (tokenId & MASK_TID_PCAT) {
    case BLTYPE_PRIMARY:
      operand = createNumberOperand (tokens);
      break;

    case TID_CAT_INSTRUCTION:
      if ((tokenId & MASK_TID_SEGMENT_OVERRIDE) == TID_P_ES_O) {
        m_segmentOverrideIID = tokenId >> WIDTH_BLTYPE + WIDTH_TID_PCAT;
        token = (token_t*) tokens->next ();
        if (token->id () == TID_S_BRACKET_OPEN) {
          operand = createPointerOperand (tokens);
        } else {
          asmErrors::exit ("\x91[\x92 expected", token);
        }
      } else {
        asmErrors::exit ("operand expected", token);
      }
      break;

    case TID_CAT_PTRDETERMINER: {
      operandSize_t operandSize = (tokenId & MASK_TID_PWIDE) != 0 ? OPERAND_SIZE_16_BITS : OPERAND_SIZE_8_BITS;
      token = (token_t*) tokens->next ();
      if ((token->id () & MASK_TID_SEGMENT_OVERRIDE) == TID_P_ES_O) {
        m_segmentOverrideIID = tokenId >> WIDTH_BLTYPE + WIDTH_TID_PCAT;
        token = (token_t*) tokens->next ();
      }
      if (token->id () == TID_S_BRACKET_OPEN) {
        operand = createPointerOperand (tokens, operandSize);
      } else {
        asmErrors::exit ("\x91[\x92 expected", token);
      }
    } break;

    case TID_CAT_REGISTER: {
      int regId = tokenId >> WIDTH_BLTYPE + WIDTH_TID_PCAT + WIDTH_TID_PWIDE;
      operandSize_t operandSize = (tokenId & MASK_TID_PWIDE) != 0 ? OPERAND_SIZE_16_BITS : OPERAND_SIZE_8_BITS;
      operand = new ops::register_t (regId, operandSize);
    } break;

    case TID_CAT_SEGREGISTER: {
      int regId = tokenId >> WIDTH_BLTYPE + WIDTH_TID_PCAT + WIDTH_TID_PWIDE;
      operand = new ops::segregister_t (regId);
    } break;
    }
    break;

  case BLTYPE_SECONDARY:
    switch (tokenId) {
    case TID_S_PARENTHESIS_OPEN:
      operand = createNumberOperand (tokens);
      break;

    case TID_S_BRACKET_OPEN:
      operand = createPointerOperand (tokens);
      break;

    default:
      /* TODO: What about expressions like ‘-(a + b)’ ? */
      asmErrors::exit ("stray \x91%t\x92 in programme", token);
    }
    break;

  case BLTYPE_STRING:
    asmErrors::exit ("strings cannot be used as operand", token);
  }

  ___CBTPOP;
  return operand;
}

operand_t* operandFactory_t::createPointerOperand (listIterator_t* tokens, operandSize_t size) {
  ___CBTPUSH;

  operand_t* operand;
  xTree_t* expression = createExpressionTree (tokens);
  token_t* token = (token_t*) tokens->next ();
  checkForPrematureEndOfFile (token);
  if (token->id () != TID_S_BRACKET_CLOSE) {
    asmErrors::exit ("expected \x91]\x92 before \x91%t\x92 token", token);
  }

  int combination = 0;
  const int regsInExpression = m_regLeavesInTree->length ();
  for (int i = 0; i < regsInExpression; ++i) {
    xConstLeaf_t* regLeaf = (xConstLeaf_t*) m_regLeavesInTree->get (i);
    token = (token_t*) m_regTokensInExpression->get (i);
    checkRegLocationInExpression (regLeaf, token);
    if ((combination & regLeaf->value ()) != 0) {
      asmErrors::exit ("wrong combination of registers", token);
    }
    combination |= regLeaf->value ();

    if (!regLeaf->isRoot ()) {
      if (regLeaf->parent ()->leftChild () == regLeaf) {
        regLeaf->parent ()->setLeftChild (m_zeroLeaf);
      } else {
        regLeaf->parent ()->setRightChild (m_zeroLeaf);
      }
      m_zeroLeaf->reserve ();
    } else {
      expression = NULL;
    }
  }

  switch (combination) {
  case 0: operand = new ops::pointer_t (6, expression, size); break;
  case 0xDD: operand = new ops::pointer_t (0x80, expression, size); break;
  case 0xD7: operand = new ops::pointer_t (0x81, expression, size); break;
  case 0x7D: operand = new ops::pointer_t (0x82, expression, size); break;
  case 0x77: operand = new ops::pointer_t (0x83, expression, size); break;
  case 0x0D: operand = new ops::pointer_t (0x84, expression, size); break;
  case 0x07: operand = new ops::pointer_t (0x85, expression, size); break;
  case 0x70: operand = new ops::pointer_t (0x86, expression, size); break;
  case 0xD0: operand = new ops::pointer_t (0x87, expression, size);
  }

  m_regTokensInExpression->clear ();
  m_regLeavesInTree->clear ();

  ___CBTPOP;
  return operand;
}

bool operandFactory_t::labelsUsed (void) {
  return m_labelsUsed;
}

token_t* operandFactory_t::operandToken (void) {
  return wm_operandToken;
}

void operandFactory_t::reset (void) {
  ___CBTPUSH;

  m_segmentOverrideIID = 0;
  m_labelsUsed = false;

  ___CBTPOP;
}

int operandFactory_t::segmentOverrideIID (void) {
  return m_segmentOverrideIID;
}

static void checkForPrematureEndOfFile (token_t* token) {
  ___BTPUSH;

  if (token->contextCode () == CTXT_LAST_TOKEN) {
    asmErrors::exit ("premature end of file");
  }

  ___BTPOP;
}

static void checkRegLocationInExpression (xConstLeaf_t* leaf, token_t* token) {
  ___BTPUSH;

  xTree_t* tree = leaf;
  while (!tree->isRoot ()) {
    xBranch_t* branch = tree->parent ();
    if (branch->precedence () > ADD_PRECEDENCE ||
        branch->getClass () == classof (xSubBranch_t) && branch->rightChild () == tree) {
      asmErrors::exit ("illegal location for register in expression", token);
    }
    tree = branch;
  }

  ___BTPOP;
}
