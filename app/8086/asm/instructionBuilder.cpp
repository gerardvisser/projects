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

#include "tokenTypes.h"
#include "../registers.h"
#include "instructionCodes.h"
#include "instructionBuilder.h"
#include <lombok/base/errors.h>

REFL_IMPL (instructionBuilder_t, object_t)

static bool sameSize (operand_t* operand1, operand_t* operand2);

instructionBuilder_t::instructionBuilder_t (void) {
  ___CBTPUSH;
  ___CBTPOP;
}

instructionBuilder_t::~instructionBuilder_t (void) {
  ___CBTPUSH;
  ___CBTPOP;
}

instruction_t* instructionBuilder_t::createAam (error_t* error, listIterator_t* tokens, int opcode) {
  ___CBTPUSH;

  operand_t* operand;
  instruction_t* instruction = NULL;
  if (((token_t*) tokens->peek ())->type == TTP_LAST) {
    operand = new operand_t (OPERAND_TYPE_IMMEDIATE, 10);
  } else {
    operand = createByteOperand (error, tokens);
  }
  if (error->code == 0) {
    instruction = noFurtherInputAllowed (error, tokens, GRP_AAM, opcode, operand);
    freeOperandsIfNecessary (instruction, operand);
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::createAdd (error_t* error, listIterator_t* tokens, int group, int opcode) {
  ___CBTPUSH;

  operand_t* operand2 = NULL;
  instruction_t* instruction = NULL;
  operand_t* operand1 = createOperand (error, tokens);
  if (error->code == 0) {

    switch (operand1->type ()) {
    case OPERAND_TYPE_POINTER:
    case OPERAND_TYPE_REGISTER: {
      token_t* token = (token_t*) tokens->next ();
      if (token->type == TTP_CHAR && token->value == ',') {
        operand2 = createOperand (error, tokens);
        if (error->code == 0) {

          if (!operand2->isSegRegister () && sameSize (operand1, operand2) && !(operand1->isPointer () && operand2->isPointer ())) {
            instruction = noFurtherInputAllowed (error, tokens, group, opcode, operand1, operand2);
          } else {
            setErrorValues (error, operandLocation);
          }

        }
      } else {
        setErrorValues (error, token->position);
      }
    } break;

    default:
      setErrorValues (error, operandLocation);
    }
    freeOperandsIfNecessary (instruction, operand1, operand2);

  }

  ___CBTPOP;
  return instruction;
}

operand_t* instructionBuilder_t::createByteOperand (error_t* error, listIterator_t* tokens) {
  ___CBTPUSH;

  operand_t* operand;
  token_t* token = (token_t*) tokens->next ();

  if (token->type == TTP_NUMBER && token->value < 0x100) {
    operand = new operand_t (OPERAND_TYPE_IMMEDIATE, token->value);
    operand->setSize (OPERAND_SIZE_8_BITS);
  } else {
    setErrorValues (error, token->position);
    operand = NULL;
  }

  ___CBTPOP;
  return operand;
}

instruction_t* instructionBuilder_t::createCall (error_t* error, listIterator_t* tokens, int group, int opcode) {
  ___CBTPUSH;

  operand_t* operand2 = NULL;
  instruction_t* instruction = NULL;
  operand_t* operand1 = createOperand (error, tokens);
  if (error->code == 0) {

    switch (operand1->type ()) {
    case OPERAND_TYPE_IMMEDIATE:
      if ((opcode & 1) == 0) {
        operand1->setSize (OPERAND_SIZE_16_BITS);
        token_t* token = (token_t*) tokens->peek ();
        if (token->type == TTP_CHAR && token->value == ':') {
          tokens->next ();
          operand2 = createOperand (error, tokens);
          if (error->code == 0) {
            if (operand2->isImmediate ()) {
              operand2->setSize (OPERAND_SIZE_16_BITS);
            } else {
              setErrorValues (error, operandLocation);
            }
          }
        }
      } else {
        setErrorValues (error, operandLocation);
      }
      break;

    case OPERAND_TYPE_POINTER:
      if (operand1->size () == OPERAND_SIZE_8_BITS || opcode & 1 && operand1->size () != OPERAND_SIZE_UNKNOWN) {
        setErrorValues (error, operandLocation);
      }
      break;

    case OPERAND_TYPE_REGISTER:
      if (opcode & 1 || operand1->size () == OPERAND_SIZE_8_BITS) {
        setErrorValues (error, operandLocation);
      }
      break;

    default:
      setErrorValues (error, operandLocation);
    }

    if (error->code == 0) {
      instruction = noFurtherInputAllowed (error, tokens, group, opcode, operand1, operand2);
    }
    freeOperandsIfNecessary (instruction, operand1, operand2);

  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::createConditionalJump (error_t* error, listIterator_t* tokens, int opcode, const address_t* address) {
  ___CBTPUSH;

  instruction_t* instruction = NULL;
  token_t* token = (token_t*) tokens->next ();

  if (token->type == TTP_NUMBER) {
    int value = token->value - address->offset () - 2;
    if (value >= -0x80 && value < 0x80) {
      operand_t* operand = new operand_t (OPERAND_TYPE_IMMEDIATE, value & 0xFF);
      operand->setSize (OPERAND_SIZE_8_BITS);
      instruction = noFurtherInputAllowed (error, tokens, GRP_CONJMP, opcode, operand);
      freeOperandsIfNecessary (instruction, operand);
    } else {
      setErrorValues (error, token->position);
    }
  } else {
    setErrorValues (error, token->position);
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::createIn (error_t* error, listIterator_t* tokens, int opcode) {
  ___CBTPUSH;

  operand_t* operand2 = NULL;
  instruction_t* instruction = NULL;
  operand_t* operand1 = createOperand (error, tokens);
  if (error->code == 0) {

    if (operand1->isAccumulator ()) {
      token_t* token = (token_t*) tokens->next ();
      if (token->type == TTP_CHAR && token->value == ',') {
        operand2 = createPortOperand (error, tokens);
        if (error->code == 0) {
          instruction = noFurtherInputAllowed (error, tokens, GRP_IN, opcode, operand1, operand2);
        }
      } else {
        setErrorValues (error, token->position);
      }
    } else {
      setErrorValues (error, operandLocation);
    }
    freeOperandsIfNecessary (instruction, operand1, operand2);

  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::createInt (error_t* error, listIterator_t* tokens, int opcode) {
  ___CBTPUSH;

  instruction_t* instruction = NULL;
  operand_t* operand = createByteOperand (error, tokens);
  if (error->code == 0) {
    instruction = noFurtherInputAllowed (error, tokens, GRP_INT, opcode, operand);
    freeOperandsIfNecessary (instruction, operand);
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::createLea (error_t* error, listIterator_t* tokens, int opcode) {
  ___CBTPUSH;

  operand_t* operand2 = NULL;
  instruction_t* instruction = NULL;
  operand_t* operand1 = createOperand (error, tokens);
  if (error->code == 0) {

    if (operand1->isRegister () && operand1->size () == OPERAND_SIZE_16_BITS) {
      token_t* token = (token_t*) tokens->next ();
      if (token->type == TTP_CHAR && token->value == ',') {
        operand2 = createOperand (error, tokens);
        if (error->code == 0) {

          if (operand2->isPointer () && operand2->size () == OPERAND_SIZE_UNKNOWN) {
            instruction = noFurtherInputAllowed (error, tokens, GRP_LEA, opcode, operand1, operand2);
          } else {
            setErrorValues (error, operandLocation);
          }

        }
      } else {
        setErrorValues (error, token->position);
      }
    } else {
      setErrorValues (error, operandLocation);
    }
    freeOperandsIfNecessary (instruction, operand1, operand2);

  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::createMov (error_t* error, listIterator_t* tokens, int opcode) {
  ___CBTPUSH;

  operand_t* operand2 = NULL;
  instruction_t* instruction = NULL;
  operand_t* operand1 = createOperand (error, tokens);
  if (error->code == 0) {

    switch (operand1->type ()) {
    case OPERAND_TYPE_POINTER:
    case OPERAND_TYPE_REGISTER:
    case OPERAND_TYPE_SEGREGISTER: {
      token_t* token = (token_t*) tokens->next ();
      if (token->type == TTP_CHAR && token->value == ',') {
        operand2 = createOperand (error, tokens);
        if (error->code == 0) {

          if (sameSize (operand1, operand2) &&
              !(operand1->isPointer () && operand2->isPointer ()) &&
              !(operand1->isSegRegister () && (operand2->isImmediate () || operand2->isSegRegister ()))) {
            instruction = noFurtherInputAllowed (error, tokens, GRP_MOV, opcode, operand1, operand2);
          } else {
            setErrorValues (error, operandLocation);
          }

        }
      } else {
        setErrorValues (error, token->position);
      }
    } break;

    default:
      setErrorValues (error, operandLocation);
    }
    freeOperandsIfNecessary (instruction, operand1, operand2);

  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::createNot (error_t* error, listIterator_t* tokens, int group, int opcode) {
  ___CBTPUSH;

  instruction_t* instruction = NULL;
  operand_t* operand = createOperand (error, tokens);
  if (error->code == 0) {
    switch (operand->type ()) {
    case OPERAND_TYPE_POINTER:
      if (operand->size () != OPERAND_SIZE_UNKNOWN) {
        instruction = noFurtherInputAllowed (error, tokens, group, opcode, operand);
      } else {
        setErrorValues (error, operandLocation);
      }
      break;

    case OPERAND_TYPE_REGISTER:
      instruction = noFurtherInputAllowed (error, tokens, group, opcode, operand);
      break;

    default:
      setErrorValues (error, operandLocation);
    }
    freeOperandsIfNecessary (instruction, operand);
  }

  ___CBTPOP;
  return instruction;
}

operand_t* instructionBuilder_t::createNumberOperand (token_t* token, bool sign) {
  ___CBTPUSH;

  int val = token->value;
  if (sign) {
    val ^= 0xFFFF;
    ++val;
    val &= 0xFFFF;
  }
  operand_t* operand = new operand_t (OPERAND_TYPE_IMMEDIATE, val);
  if (token->value > 0xFF) {
    operand->setSize (OPERAND_SIZE_16_BITS);
  }

  ___CBTPOP;
  return operand;
}

operand_t* instructionBuilder_t::createOperand (error_t* error, listIterator_t* tokens) {
  ___CBTPUSH;

  operand_t* operand = NULL;
  token_t* token = (token_t*) tokens->next ();
  setOperandLocation (token);

  switch (token->type) {
  case TTP_CHAR:
    switch (token->value) {
    case '[': operand = createPointerOperand (error, tokens); break;

    case '-':
    case '+': {
      bool sign = token->value == '-';
      token = (token_t*) tokens->next ();
      if (token->type == TTP_NUMBER) {
        operand = createNumberOperand (token, sign);
      } else {
        setErrorValues (error, token->position);
      }
    } break;

    default: setErrorValues (error, token->position); break;
    }
    break;

  case TTP_NUMBER: operand = createNumberOperand (token); break;

  case TTP_PTRDETERMINER: {
    operandSize_t operandSize = token->value == 1 ? OPERAND_SIZE_16_BITS : OPERAND_SIZE_8_BITS;
    token = (token_t*) tokens->next ();
    if (token->type == TTP_CHAR && token->value == '[') {
      operand = createPointerOperand (error, tokens);
      if (operand != NULL) {
        operand->setSize (operandSize);
      }
    } else {
      setErrorValues (error, token->position);
    }
  } break;

  case TTP_REGISTER: {
    operandSize_t operandSize = (token->value & 8) != 0 ? OPERAND_SIZE_16_BITS : OPERAND_SIZE_8_BITS;
    operand = new operand_t (OPERAND_TYPE_REGISTER, token->value & 7);
    operand->setSize (operandSize);
  } break;

  case TTP_SEGREGISTER:
    operand = new operand_t (OPERAND_TYPE_SEGREGISTER, token->value);
    operand->setSize (OPERAND_SIZE_16_BITS);
    break;

  default: setErrorValues (error, token->position);
  }

  ___CBTPOP;
  return operand;
}

instruction_t* instructionBuilder_t::createOut (error_t* error, listIterator_t* tokens, int opcode) {
  ___CBTPUSH;

  operand_t* operand2 = NULL;
  instruction_t* instruction = NULL;
  operand_t* operand1 = createPortOperand (error, tokens);
  if (error->code == 0) {

    token_t* token = (token_t*) tokens->next ();
    if (token->type == TTP_CHAR && token->value == ',') {
      operand2 = createOperand (error, tokens);
      if (error->code == 0) {

        if (operand2->isAccumulator ()) {
          instruction = noFurtherInputAllowed (error, tokens, GRP_OUT, opcode, operand1, operand2);
        } else {
          setErrorValues (error, operandLocation);
        }

      }
    } else {
      setErrorValues (error, token->position);
    }
    freeOperandsIfNecessary (instruction, operand1, operand2);

  }

  ___CBTPOP;
  return instruction;
}

/*
Only the following register combinations to calculate an address from are possible:
BX+SI, BX+DI, BP+SI, BP+DI, SI, DI, BP, BX. Note that the registers AX, CX, DX, SP
never occur and that BX cannot occur in combination with BP and that SI can't occur
in combination with DI. Also immediate values are allowed; any number of them because
they can be combined at compile-time.
    We're confronted by any sequence of tokens. The tokens will be examined one by
one until the ending ]-character occurs or until an error occurs. I will try to con-
struct an easy and resource-friendly way to determine whether the token at hand is
legal or not.
    Some state of which registers have been met already needs to be kept. Let this
be an integer. The registers that can never occur will be represented by 1 bit, the
ones that are allowed (once, and only in the combinations shown) are represented by
2 bits:

DI SI BP SP BX DX CX AX
.. .. .. .  .. .  .  .

The first bit for each register determines whether the register is still allowed
(when the bit is clear), the second whether it has been selected. The start state
now consists of four bits being set (for AX, CX, DX, SP) and the other eight bits
are clear: 00 00 00 1 00 1 1 1 (0x27).
    Now the masks can be determined easily: AX, CX, DX and SP simply have their bit
set. Each of the other registers has their selection and still-allowed bit set, and
the still-allowed bit of the other register in combination with which it cannot
occur.
    Two examples:
DI SI BP SP BX DX CX AX
00 00 11 0  01 0  0  0   BP-mask. (0xC8)
00 00 00 1  00 0  0  0   SP-mask. (0x20)

    If no errors occur, the final state is AND-ed with a mask that singles out the
four selection bits:
10 10 10 0  10 0  0  0:  0xA90   */
operand_t* instructionBuilder_t::createPointerOperand (error_t* error, listIterator_t* tokens) {
  ___CBTPUSH;

  int state = 0x27;
  const int mask[] = {0x1, 0x2, 0x4, 0x58, 0x20, 0xC8, 0x700, 0xD00, 0xA90};

  unsigned short displacement = 0;

  operand_t* operand = NULL;
  token_t* token = (token_t*) tokens->next ();

  do {
    bool sign = false;
    if (token->type == TTP_CHAR && (token->value == '+' || token->value == '-')) {
      sign = token->value == '-';
      token = (token_t*) tokens->next ();
    }
    if (token->type == TTP_NUMBER) {
      if (sign) {
        displacement -= token->value;
      } else {
        displacement += token->value;
      }
      token = (token_t*) tokens->next ();
    } else if (!sign && token->type == TTP_REGISTER && token->value > 7) {
      if ((state & mask[token->value - 8]) == 0) {
        state |= mask[token->value - 8];
        token = (token_t*) tokens->next ();
      } else {
        setErrorValues (error, token->position);
      }
    } else {
      setErrorValues (error, token->position);
    }
  } while (error->code == 0 && !(token->type == TTP_CHAR && token->value == ']'));

  if (error->code == 0) {
    int modrm;
    if (displacement == 0) {
      modrm = 0;
    } else if (displacement < 0x80 || displacement > 0xFF7F) {
      modrm = 0x40;
    } else {
      modrm = 0x80;
    }

    switch (state & mask[8]) {
    case 0: modrm = 6; break; /* No registers were selected. */
    //case 0x210: modrm |= 0; break; /* BX+SI */
    case 0x810: modrm |= 1; break; /* BX+DI */
    case 0x280: modrm |= 2; break; /* BP+SI */
    case 0x880: modrm |= 3; break; /* BP+DI */
    case 0x200: modrm |= 4; break; /* SI */
    case 0x800: modrm |= 5; break; /* DI */
    case 0x80:
      modrm |= 6;
      if (modrm == 6) { /* See case 0. */
        modrm = 0x46;
      }
      break; /* BP */
    case 0x10: modrm |= 7; break; /* BX */
    }

    operand = new operand_t (OPERAND_TYPE_POINTER, modrm, displacement);
  }

  ___CBTPOP;
  return operand;
}

operand_t* instructionBuilder_t::createPortOperand (error_t* error, listIterator_t* tokens) {
  ___CBTPUSH;

  operand_t* operand;
  token_t* token = (token_t*) tokens->next ();

  if (token->type == TTP_NUMBER && token->value < 0x100) {
    operand = new operand_t (OPERAND_TYPE_IMMEDIATE, token->value);
    operand->setSize (OPERAND_SIZE_8_BITS);
  } else if (token->type == TTP_REGISTER && token->value == DX + 8) {
    operand = new operand_t (OPERAND_TYPE_REGISTER, DX);
    operand->setSize (OPERAND_SIZE_16_BITS);
  } else {
    setErrorValues (error, token->position);
    operand = NULL;
  }

  ___CBTPOP;
  return operand;
}

instruction_t* instructionBuilder_t::createPush (error_t* error, listIterator_t* tokens, int opcode) {
  ___CBTPUSH;

  instruction_t* instruction = NULL;
  operand_t* operand = createOperand (error, tokens);
  if (error->code == 0) {
    switch (operand->type ()) {
    case OPERAND_TYPE_POINTER:
    case OPERAND_TYPE_REGISTER:
      if (operand->size () != OPERAND_SIZE_8_BITS) {
        instruction = noFurtherInputAllowed (error, tokens, GRP_PUSH, opcode, operand);
        operand->setSize (OPERAND_SIZE_16_BITS);
      } else {
        setErrorValues (error, operandLocation);
      }
      break;

    case OPERAND_TYPE_SEGREGISTER:
      if (!(opcode == OPC_POP && operand->value () == CS)) {
        instruction = noFurtherInputAllowed (error, tokens, GRP_PUSH, opcode, operand);
      } else {
        setErrorValues (error, operandLocation);
      }
      break;

    default:
      setErrorValues (error, operandLocation);
    }
    freeOperandsIfNecessary (instruction, operand);
  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::createRet (error_t* error, listIterator_t* tokens, int opcode) {
  ___CBTPUSH;

  operand_t* operand = NULL;
  instruction_t* instruction = NULL;
  if (((token_t*) tokens->peek ())->type != TTP_LAST) {
    operand = createOperand (error, tokens);
    if (error->code == 0) {
      if (operand->isImmediate ()) {
        operand->setSize (OPERAND_SIZE_16_BITS);
      } else {
        setErrorValues (error, operandLocation);
      }
    }
  }
  if (error->code == 0) {
    instruction = noFurtherInputAllowed (error, tokens, GRP_RET, opcode, operand);
  }
  freeOperandsIfNecessary (instruction, operand);

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::createShift (error_t* error, listIterator_t* tokens, int opcode) {
  ___CBTPUSH;

  operand_t* operand2 = NULL;
  instruction_t* instruction = NULL;
  operand_t* operand1 = createOperand (error, tokens);
  if (error->code == 0) {

    if (operand1->isRegister () || operand1->isPointer () && operand1->size () != OPERAND_SIZE_UNKNOWN) {
      token_t* token = (token_t*) tokens->next ();
      if (token->type == TTP_CHAR && token->value == ',') {
        operand2 = createOperand (error, tokens);
        if (error->code == 0) {

          if (operand2->value () == 1 && (operand2->isImmediate () || operand2->isRegister () && !operand2->isWide ())) {
            instruction = noFurtherInputAllowed (error, tokens, GRP_SHIFT, opcode, operand1, operand2);
            operand2->setSize (OPERAND_SIZE_8_BITS);
          } else {
            setErrorValues (error, operandLocation);
          }

        }
      } else {
        setErrorValues (error, token->position);
      }
    } else {
      setErrorValues (error, operandLocation);
    }
    freeOperandsIfNecessary (instruction, operand1, operand2);

  }

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::createXchg (error_t* error, listIterator_t* tokens, int opcode) {
  ___CBTPUSH;

  operand_t* operand2 = NULL;
  instruction_t* instruction = NULL;
  operand_t* operand1 = createOperand (error, tokens);
  if (error->code == 0) {

    switch (operand1->type ()) {
    case OPERAND_TYPE_POINTER:
    case OPERAND_TYPE_REGISTER: {
      token_t* token = (token_t*) tokens->next ();
      if (token->type == TTP_CHAR && token->value == ',') {
        operand2 = createOperand (error, tokens);
        if (error->code == 0) {

          if (sameSize (operand1, operand2) && (operand2->isRegister () || operand2->isPointer () && !operand1->isPointer ())) {
            instruction = noFurtherInputAllowed (error, tokens, GRP_XCHG, opcode, operand1, operand2);
          } else {
            setErrorValues (error, operandLocation);
          }

        }
      } else {
        setErrorValues (error, token->position);
      }
    } break;

    default:
      setErrorValues (error, operandLocation);
    }
    freeOperandsIfNecessary (instruction, operand1, operand2);

  }

  ___CBTPOP;
  return instruction;
}

void instructionBuilder_t::freeOperandsIfNecessary (instruction_t* instruction, operand_t* operand1, operand_t* operand2) {
  ___CBTPUSH;

  if (instruction == NULL) {
    if (operand1 != NULL) {
      operand1->dump ();
      if (operand2 != NULL) {
        operand2->dump ();
      }
    }
  }

  ___CBTPOP;
}

/* Restrictions on the list of tokens:
   1. The list contains at least two items.
   2. The first token is of type TTP_INSTRUCTION.
   3. The last token is of type TTP_LAST. */
instruction_t* instructionBuilder_t::getInstruction (error_t* error, list_t* tokens, const address_t* address) {
  ___CBTPUSH;

  reset ();
  error->code = 0;
  instruction_t* instruction;
  listIterator_t* iter = tokens->iterator ();
  int instrId = ((token_t*) iter->next ())->value;
  int opcode = instrId & 0xFF;
  int group = instrId >> 8;
  switch (group) {
  case GRP_AAM: instruction = createAam (error, iter, opcode); break;
  case GRP_ADD:
  case GRP_TEST: instruction = createAdd (error, iter, group, opcode); break;
  case GRP_CALL:
  case GRP_JMP: instruction = createCall (error, iter, group, opcode); break;
  case GRP_CONJMP: instruction = createConditionalJump (error, iter, opcode, address); break;
  case GRP_IN: instruction = createIn (error, iter, opcode); break;
  case GRP_INC:
  case GRP_NOT: instruction = createNot (error, iter, group, opcode); break;
  case GRP_INT: instruction = createInt (error, iter, opcode); break;
  case GRP_LEA: instruction = createLea (error, iter, opcode); break;
  case GRP_MISC: instruction = noFurtherInputAllowed (error, iter, GRP_MISC, opcode); break;
  case GRP_MOV: instruction = createMov (error, iter, opcode); break;
  case GRP_OUT: instruction = createOut (error, iter, opcode); break;
  case GRP_PUSH: instruction = createPush (error, iter, opcode); break;
  case GRP_RET: instruction = createRet (error, iter, opcode); break;
  case GRP_SHIFT: instruction = createShift (error, iter, opcode); break;
  case GRP_XCHG: instruction = createXchg (error, iter, opcode);
  }
  iter->dump ();

  ___CBTPOP;
  return instruction;
}

instruction_t* instructionBuilder_t::noFurtherInputAllowed (error_t* error,
                                                     listIterator_t* tokens,
                                                                int  group,
                                                                int  opcode,
                                                          operand_t* operand1,
                                                          operand_t* operand2) {
  ___CBTPUSH;

  instruction_t* instruction = NULL;
  token_t* token = (token_t*) tokens->next ();
  if (token->type == TTP_LAST) {
    instruction = new instruction_t (group, opcode, operand1, operand2);
  } else {
    setErrorValues (error, token->position);
  }

  ___CBTPOP;
  return instruction;
}

void instructionBuilder_t::reset (void) {
  ___CBTPUSH;

  operandLocation = 0;

  ___CBTPOP;
}

void instructionBuilder_t::setErrorValues (error_t* error, int location) {
  ___CBTPUSH;

  error->location = location;
  error->code = 1;

  ___CBTPOP;
}

void instructionBuilder_t::setOperandLocation (token_t* token) {
  ___CBTPUSH;

  operandLocation = token->position;

  ___CBTPOP;
}

static bool sameSize (operand_t* operand1, operand_t* operand2) {
  ___BTPUSH;

  bool result;
  operandSize_t size1 = operand1->size ();
  operandSize_t size2 = operand2->size ();
  if (size1 == OPERAND_SIZE_UNKNOWN) {
    result = size2 != OPERAND_SIZE_UNKNOWN;
    if (result) {
      operand1->setSize (size2);
    }
  } else if (size2 == OPERAND_SIZE_UNKNOWN) {
    operand2->setSize (size1);
    result = true;
  } else {
    result = size1 == size2;
  }

  ___BTPOP;
  return result;
}
