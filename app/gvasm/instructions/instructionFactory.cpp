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
#include <lombok/util/stringBuilder.h>
#include "../asmErrors.h"
#include "../asmTokenIDs.h"
#include "../data.h"
#include "all.h"

REFL_IMPL (instructionFactory_t, object_t)

using namespace pars;

instructionFactory_t::instructionFactory_t (hashMap_t* labels) {
  ___CBTPUSH;

  m_operandFactory = new operandFactory_t (labels);

  ___CBTPOP;
}

instructionFactory_t::~instructionFactory_t (void) {
  ___CBTPUSH;

  m_operandFactory->dump ();

  ___CBTPOP;
}

void instructionFactory_t::assertNextIsComma (listIterator_t* tokens) {
  ___CBTPUSH;

  token_t* token = (token_t*) tokens->next ();
  if (token->id () != TID_S_COMMA) {
    asmErrors::exit ("\x91,\x92 expected", token);
  }

  ___CBTPOP;
}

void instructionFactory_t::assertSizesAreEqual (operand_t* operand1, operand_t* operand2) {
  ___CBTPUSH;

  operandSize_t size1 = operand1->m_size;
  operandSize_t size2 = operand2->m_size;
  if (size1 == OPERAND_SIZE_UNKNOWN) {
    if (size2 != OPERAND_SIZE_UNKNOWN) {
      operand1->m_size = size2;
    } else {
      asmErrors::exit ("operand size unknown", wm_instructionToken);
    }
  } else if (size2 == OPERAND_SIZE_UNKNOWN) {
    operand2->m_size = size1;
  } else if (size1 != size2) {
    asmErrors::exit ("operands not of same size", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
}

instruction_t* instructionFactory_t::createAam (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  operand_t* operand;
  token_t* token = (token_t*) tokens->peek ();
  switch (token->id () & MASK_BLTYPE) {
  case BLTYPE_CHAR:
  case BLTYPE_NUMBER: {
    operand = m_operandFactory->createOperand (tokens);
    int value = operand->value ();
    if (value < 1 || value > 255 || m_operandFactory->labelsUsed ()) {
      asmErrors::exit ("optional operand of \x91%s\x92 should be a number ranging from 1 to 255 (label usage not allowed)",
                       m_operandFactory->operandToken (),
                       wm_instructionToken->text ()->toUtf8 ());
    }
  } break;

  default:
    operand = NULL;
  }
  instruction_t* instruction = new fixedSizeInstruction_t (csOffset, instrId, 2, operand);

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createAdd (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand1 = m_operandFactory->createOperand (tokens);
  switch (operand1->type ()) {
  case OPERAND_TYPE_POINTER:
  case OPERAND_TYPE_REGISTER: {
    assertNextIsComma (tokens);
    operand_t* operand2 = m_operandFactory->createOperand (tokens);

    if (operand1->isPointer () && operand2->isPointer ()) {
      asmErrors::exit ("an instruction cannot have two memory operands", m_operandFactory->operandToken ());
    } else if (operand2->isSegRegister ()) {
      asmErrors::exit ("instruction \x91%s\x92 cannot have a segment register as its operand", m_operandFactory->operandToken (), wm_instructionToken->text ()->toUtf8 ());
    }
    assertSizesAreEqual (operand1, operand2);

    if (!operand2->isImmediate ()) {
      instruction = new modrmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand1, operand2);
    } else if (operand1->isAccumulator ()) {
      instruction = new immInstruction_t (csOffset, instrId, operand1, operand2);
    } else {
      instruction = new modrmImmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand1, operand2);
    }
  } break;

  default:
    asmErrors::exit ("illegal operand: memory or register operand expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createCall (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand1 = m_operandFactory->createOperand (tokens);
  switch (operand1->type ()) {
  case OPERAND_TYPE_IMMEDIATE: {
    token_t* token = (token_t*) tokens->next ();
    operand1->m_size = OPERAND_SIZE_16_BITS;
    if (token->id () == TID_S_COLON) {
      operand_t* operand2 = m_operandFactory->createOperand (tokens);
      if (!operand2->isImmediate ()) {
        asmErrors::exit ("offset value expected", m_operandFactory->operandToken ());
      }
      operand2->m_size = OPERAND_SIZE_16_BITS;
      instruction = new fixedSizeInstruction_t (csOffset, instrId, 5, operand1, operand2); /* Maybe FAR-part can be removed... */
    } else if (instrId >> WIDTH_IID_GRP & 1) {
      asmErrors::exit ("\x91:\x92 expected", token);
    } else if (instrId == IID_CALL) {
      instruction = new fixedSizeInstruction_t (csOffset, instrId, 3, operand1);
      tokens->previous ();
    } else {
      instruction = new locInstruction_t (csOffset, instrId, operand1);
      tokens->previous ();
    }
  } break;

  case OPERAND_TYPE_POINTER:
  case OPERAND_TYPE_REGISTER:
    if (instrId >> WIDTH_IID_GRP & 1) {
      if (operand1->size () != OPERAND_SIZE_UNKNOWN) {
        asmErrors::exit ("32 bits operand expected", m_operandFactory->operandToken ());
      }
    } else {
      if (operand1->size () == OPERAND_SIZE_8_BITS) {
        asmErrors::exit ("16 bits operand expected", m_operandFactory->operandToken ());
      }
      operand1->m_size = OPERAND_SIZE_16_BITS;
    }
    instruction = new modrmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand1);
    break;

  default:
    asmErrors::exit ("illegal operand: memory, register or immediate operand expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createConditionalJump (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand = m_operandFactory->createOperand (tokens);
  if (operand->type () == OPERAND_TYPE_IMMEDIATE) {
    instruction = new conditionalJump_t (csOffset, instrId, operand, wm_instructionToken->beginLine (), wm_instructionToken->beginColumn ());
    operand->m_size = OPERAND_SIZE_16_BITS;
  } else {
    asmErrors::exit ("a nearby location to jump to expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

data_t* instructionFactory_t::createData (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  data_t* data;
  switch (instrId) {
  case IID_DB: data = m_operandFactory->createData (tokens, data_t::UNIT_SIZE_8_BITS, csOffset); break;
  case IID_DW: data = m_operandFactory->createData (tokens, data_t::UNIT_SIZE_16_BITS, csOffset); break;
  case IID_DC: {
    string_t* tokenText = ((token_t*) tokens->next ())->text ();
    const int loopMax = tokenText->length () - 1;
    stringBuilder_t* text = new stringBuilder_t (loopMax);
    bool bsl = false;
    for (int i = 1; i < loopMax; ++i) {
      uchar_t c = tokenText->charAt (i);
      if (bsl) {
        switch (c) {
        case '"':
        case '\'':
        case '\\': text->append (c); break;
        case 'b': text->append ('\b'); break;
        case 'f': text->append ('\f'); break;
        case 'n': text->append ('\n'); break;
        case 'r': text->append ('\r'); break;
        case 't': text->append ('\t'); break;
        default:
          text->append ('\\');
          text->append (c);
        }
        bsl = false;
      } else {
        bsl = c == '\\';
        if (!bsl) {
          text->append (c);
        }
      }
    }
    data = new data_t (csOffset, text);
    text->dump ();
  }
  }

  ___CBTPOP;
  return data;
}

instruction_t* instructionFactory_t::createIn (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand1 = m_operandFactory->createOperand (tokens);
  if (operand1->isAccumulator ()) {
    assertNextIsComma (tokens);
    operand_t* operand2 = m_operandFactory->createOperand (tokens);
    switch (operand2->type ()) {
    case OPERAND_TYPE_IMMEDIATE: {
      int value = operand2->value ();
      if (value < 0 || value > 255 || m_operandFactory->labelsUsed ()) {
        asmErrors::exit ("port numbers that can be immediately specified range from 0 to 255 (label usage not allowed)", m_operandFactory->operandToken ());
      }
      instruction = new fixedSizeInstruction_t (csOffset, instrId, 2, operand1, operand2);
      operand2->m_size = OPERAND_SIZE_8_BITS;
    } break;

    case OPERAND_TYPE_REGISTER:
      if (!(operand2->value () == 2 && operand2->size () == OPERAND_SIZE_16_BITS)) {
        asmErrors::exit ("a number ranging from 0 to 255 or \221dx\x92 expected", m_operandFactory->operandToken ());
      }
      instruction = new fixedSizeInstruction_t (csOffset, instrId, 1, operand1, operand2);
      break;

    default:
      asmErrors::exit ("a number ranging from 0 to 255 or \221dx\x92 expected", m_operandFactory->operandToken ());
    }
  } else {
    asmErrors::exit ("\221al\x92 or \221ax\x92 expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createInc (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand = m_operandFactory->createOperand (tokens);
  switch (operand->type ()) {
  case OPERAND_TYPE_POINTER:
  case OPERAND_TYPE_REGISTER:
    if (operand->size () == OPERAND_SIZE_UNKNOWN) {
      asmErrors::exit ("operand size unknown", m_operandFactory->operandToken ());
    } else if (operand->isRegister () && operand->size () == OPERAND_SIZE_16_BITS) {
      instruction = new fixedSizeInstruction_t (csOffset, instrId, 1, operand);
    } else {
      instruction = new modrmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand);
    }
    break;

  default:
    asmErrors::exit ("illegal operand: memory or register operand expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

byteBlock_t* instructionFactory_t::createInstruction (listIterator_t* tokens, pars::token_t* token, xValue_t* csOffset) {
  ___CBTPUSH;

  m_operandFactory->reset ();
  wm_instructionToken = token;

  byteBlock_t* byteBlock;
  int instrId = token->id () >> WIDTH_BLTYPE + WIDTH_TID_PCAT;
  switch (instrId & MASK_IID_GRP) {
  case GRP_AAM: byteBlock = createAam (tokens, csOffset, instrId); break;
  case GRP_ADD:
  case GRP_TEST: byteBlock = createAdd (tokens, csOffset, instrId); break;
  case GRP_CALL:
  case GRP_JMP: byteBlock = createCall (tokens, csOffset, instrId); break;
  case GRP_CONJMP: byteBlock = createConditionalJump (tokens, csOffset, instrId); break;
  case GRP_DATA: byteBlock = createData (tokens, csOffset, instrId); break;
  case GRP_IN: byteBlock = createIn (tokens, csOffset, instrId); break;
  case GRP_INC: byteBlock = createInc (tokens, csOffset, instrId); break;
  case GRP_INT: byteBlock = createInt (tokens, csOffset, instrId); break;
  case GRP_LEA: byteBlock = createLea (tokens, csOffset, instrId); break;
  case GRP_MISC: byteBlock = new fixedSizeInstruction_t (csOffset, instrId, 1); break;
  case GRP_MOV: byteBlock = createMov (tokens, csOffset, instrId); break;
  case GRP_NOT: byteBlock = createNot (tokens, csOffset, instrId); break;
  case GRP_OUT: byteBlock = createOut (tokens, csOffset, instrId); break;
  case GRP_PUSH: byteBlock = createPush (tokens, csOffset, instrId); break;
  case GRP_RET: byteBlock = createRet (tokens, csOffset, instrId); break;
  case GRP_SHIFT: byteBlock = createShift (tokens, csOffset, instrId); break;
  case GRP_XCHG: byteBlock = createXchg (tokens, csOffset, instrId);
  }

  ___CBTPOP;
  return byteBlock;
}

instruction_t* instructionFactory_t::createInt (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand = m_operandFactory->createOperand (tokens);
  if (operand->type () == OPERAND_TYPE_IMMEDIATE) {
    int value = operand->value ();
    if (value < 0 || value > 255 || m_operandFactory->labelsUsed ()) {
      asmErrors::exit ("an interrupt type number ranges from 0 to 255 (label usage not allowed)", m_operandFactory->operandToken ());
    }
    instruction = new fixedSizeInstruction_t (csOffset, instrId, 2, operand);
    operand->m_size = OPERAND_SIZE_8_BITS;
  } else {
    asmErrors::exit ("interrupt type number expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createLea (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  operand_t* operand1 = m_operandFactory->createOperand (tokens);
  if (!(operand1->isRegister () && operand1->size () == OPERAND_SIZE_16_BITS)) {
    asmErrors::exit ("16 bits register expected", m_operandFactory->operandToken ());
  }

  assertNextIsComma (tokens);
  operand_t* operand2 = m_operandFactory->createOperand (tokens);
  if (!operand2->isPointer ()) {
    asmErrors::exit ("memory operand expected", m_operandFactory->operandToken ());
  } else if (instrId == IID_LEA) {
    if (operand2->size () == OPERAND_SIZE_8_BITS) {
      asmErrors::exit ("16 bits memory operand expected", m_operandFactory->operandToken ());
    }
  } else if (operand2->size () != OPERAND_SIZE_UNKNOWN) {
    asmErrors::exit ("32 bits memory operand expected", m_operandFactory->operandToken ());
  }

  instruction_t* instruction = new modrmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand1, operand2);

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createMov (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand1 = m_operandFactory->createOperand (tokens);
  switch (operand1->type ()) {
  case OPERAND_TYPE_POINTER:
  case OPERAND_TYPE_REGISTER:
  case OPERAND_TYPE_SEGREGISTER: {
    assertNextIsComma (tokens);
    operand_t* operand2 = m_operandFactory->createOperand (tokens);

    if (operand1->isPointer () && operand2->isPointer ()) {
      asmErrors::exit ("an instruction cannot have two memory operands", m_operandFactory->operandToken ());
    } else if (operand1->isSegRegister () && (operand2->isImmediate () || operand2->isSegRegister ())) {
      if (operand2->isImmediate ()) {
        asmErrors::exit ("cannot copy an immediate value into a segment register", m_operandFactory->operandToken ());
      } else if (operand2->isSegRegister ()) {
        asmErrors::exit ("cannot copy a segment register's value directly into another segment register", m_operandFactory->operandToken ());
      }
    }
    assertSizesAreEqual (operand1, operand2);

    if (operand1->isPointer ()) {
      if (operand2->isAccumulator () && operand1->value () == 6) {
        instruction = new fixedSizeInstruction_t (csOffset, instrId, 3, operand1, operand2);
      } else if (operand2->isImmediate ()) {
        instruction = new modrmImmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand1, operand2);
      } else {
        instruction = new modrmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand1, operand2);
      }
    } else if (operand2->isImmediate ()) {
      instruction = new immInstruction_t (csOffset, instrId, operand1, operand2);
    } else if (operand1->isAccumulator () && operand2->isPointer () && operand2->value () == 6) {
      instruction = new fixedSizeInstruction_t (csOffset, instrId, 3, operand1, operand2);
    } else {
      instruction = new modrmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand1, operand2);
    }
  } break;

  default:
    asmErrors::exit ("illegal operand: memory, register or segment register operand expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createNot (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand = m_operandFactory->createOperand (tokens);
  switch (operand->type ()) {
  case OPERAND_TYPE_POINTER:
  case OPERAND_TYPE_REGISTER:
    if (operand->size () == OPERAND_SIZE_UNKNOWN) {
      asmErrors::exit ("operand size unknown", m_operandFactory->operandToken ());
    }
    instruction = new modrmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand);
    break;

  default:
    asmErrors::exit ("illegal operand: memory or register operand expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createOut (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  int instrSize;
  operand_t* operand1 = m_operandFactory->createOperand (tokens);
  if (operand1->isImmediate ()) {
    int value = operand1->value ();
    if (value < 0 || value > 255 || m_operandFactory->labelsUsed ()) {
      asmErrors::exit ("port numbers that can be immediately specified range from 0 to 255 (label usage not allowed)", m_operandFactory->operandToken ());
    }
    operand1->m_size = OPERAND_SIZE_8_BITS;
    instrSize = 2;
  } else if (operand1->isRegister ()) {
    if (!(operand1->value () == 2 && operand1->size () == OPERAND_SIZE_16_BITS)) {
      asmErrors::exit ("a number ranging from 0 to 255 or \221dx\x92 expected", m_operandFactory->operandToken ());
    }
    instrSize = 1;
  } else {
    asmErrors::exit ("a number ranging from 0 to 255 or \221dx\x92 expected", m_operandFactory->operandToken ());
  }

  assertNextIsComma (tokens);
  operand_t* operand2 = m_operandFactory->createOperand (tokens);
  if (!operand2->isAccumulator ()) {
    asmErrors::exit ("\221al\x92 or \221ax\x92 expected", m_operandFactory->operandToken ());
  }

  instruction_t* instruction = new fixedSizeInstruction_t (csOffset, instrId, instrSize, operand1, operand2);

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createPush (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand = m_operandFactory->createOperand (tokens);
  if (operand->size () == OPERAND_SIZE_8_BITS) {
    asmErrors::exit ("16 bits operand expected", m_operandFactory->operandToken ());
  }
  switch (operand->type ()) {
  case OPERAND_TYPE_POINTER:
    instruction = new modrmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand);
    operand->m_size = OPERAND_SIZE_16_BITS;
    break;

  case OPERAND_TYPE_REGISTER:
  case OPERAND_TYPE_SEGREGISTER:
    instruction = new fixedSizeInstruction_t (csOffset, instrId, 1, operand);
    break;

  default:
    asmErrors::exit ("illegal operand: memory, register or segment register operand expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createRet (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  token_t* token = (token_t*) tokens->peek ();
  switch (token->id () & MASK_BLTYPE) {
  case BLTYPE_CHAR:
  case BLTYPE_NUMBER: {
    operand_t* operand = m_operandFactory->createOperand (tokens);
    instruction = new fixedSizeInstruction_t (csOffset, instrId, 3, operand);
    operand->m_size = OPERAND_SIZE_16_BITS;
  } break;

  default:
    instruction = new fixedSizeInstruction_t (csOffset, instrId, 1);
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createShift (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand1 = m_operandFactory->createOperand (tokens);
  switch (operand1->type ()) {
  case OPERAND_TYPE_POINTER:
  case OPERAND_TYPE_REGISTER: {
    if (operand1->size () == OPERAND_SIZE_UNKNOWN) {
      asmErrors::exit ("operand size unknown", m_operandFactory->operandToken ());
    }
    assertNextIsComma (tokens);
    operand_t* operand2 = m_operandFactory->createOperand (tokens);
    if (!(operand2->value () == 1 && (operand2->isImmediate () || operand2->isRegister () && !operand2->isWide ()))) {
      /* Note: we cannot be certain whether an immediate value is equal to 1 or not (labels!). */
      asmErrors::exit ("\221\x31\x92 or \221cl\x92 expected", m_operandFactory->operandToken ());
    }
    instruction = new modrmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand1, operand2);
  } break;

  default:
    asmErrors::exit ("illegal operand: memory or register operand expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionFactory_t::createXchg (listIterator_t* tokens, xValue_t* csOffset, int instrId) {
  ___CBTPUSH;

  instruction_t* instruction;
  operand_t* operand1 = m_operandFactory->createOperand (tokens);
  switch (operand1->type ()) {
  case OPERAND_TYPE_POINTER:
  case OPERAND_TYPE_REGISTER: {
    assertNextIsComma (tokens);
    operand_t* operand2 = m_operandFactory->createOperand (tokens);
    if (operand1->isPointer () && operand2->isPointer ()) {
      asmErrors::exit ("an instruction cannot have two memory operands", m_operandFactory->operandToken ());
    } else if (!(operand2->isPointer () || operand2->isRegister ())) {
      asmErrors::exit ("illegal operand: memory or register operand expected", m_operandFactory->operandToken ());
    }
    assertSizesAreEqual (operand1, operand2);

    if (operand2->isAccumulator () && operand2->isWide ()) {
      operand_t* operand = operand2;
      operand2 = operand1;
      operand1 = operand;
    }
    if (operand1->isAccumulator () && operand1->isWide () && operand2->isRegister ()) {
      instruction = new fixedSizeInstruction_t (csOffset, instrId, 1, operand1, operand2);
    } else {
      instruction = new modrmInstruction_t (csOffset, instrId, !m_operandFactory->labelsUsed (), operand1, operand2);
    }
  } break;

  default:
    asmErrors::exit ("illegal operand: memory or register operand expected", m_operandFactory->operandToken ());
  }

  ___CBTPOP;
  return instruction;
}

int instructionFactory_t::segmentOverrideIID (void) {
  ___CBTPUSH;

  int result = m_operandFactory->segmentOverrideIID ();

  ___CBTPOP;
  return result;
}
