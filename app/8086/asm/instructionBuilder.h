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

#ifndef ___DEBUG_ASM_INSTRUCTION_BUILDER_INCLUDED
#define ___DEBUG_ASM_INSTRUCTION_BUILDER_INCLUDED

#include "error.h"
#include "token.h"
#include "../address.h"
#include "instruction.h"
#include <lombok/util/list.h>

class instructionBuilder_t : public object_t {
private:
  int operandLocation;

public:
  instructionBuilder_t (void);
  virtual ~instructionBuilder_t (void);

  virtual instruction_t* getInstruction (error_t* error, list_t* tokens, const address_t* address);

private:
  instruction_t* createAam (error_t* error, listIterator_t* tokens, int opcode);
  instruction_t* createAdd (error_t* error, listIterator_t* tokens, int group, int opcode);
  operand_t* createByteOperand (error_t* error, listIterator_t* tokens);
  instruction_t* createCall (error_t* error, listIterator_t* tokens, int group, int opcode);
  instruction_t* createConditionalJump (error_t* error, listIterator_t* tokens, int opcode, const address_t* address);
  instruction_t* createIn (error_t* error, listIterator_t* tokens, int opcode);
  instruction_t* createInt (error_t* error, listIterator_t* tokens, int opcode);
  instruction_t* createLea (error_t* error, listIterator_t* tokens, int opcode);
  instruction_t* createMov (error_t* error, listIterator_t* tokens, int opcode);
  instruction_t* createNot (error_t* error, listIterator_t* tokens, int group, int opcode);
  operand_t* createNumberOperand (token_t* token, bool sign = false);
  operand_t* createOperand (error_t* error, listIterator_t* tokens);
  instruction_t* createOut (error_t* error, listIterator_t* tokens, int opcode);
  operand_t* createPointerOperand (error_t* error, listIterator_t* tokens);
  operand_t* createPortOperand (error_t* error, listIterator_t* tokens);
  instruction_t* createPush (error_t* error, listIterator_t* tokens, int opcode);
  instruction_t* createRet (error_t* error, listIterator_t* tokens, int opcode);
  instruction_t* createShift (error_t* error, listIterator_t* tokens, int opcode);
  instruction_t* createXchg (error_t* error, listIterator_t* tokens, int opcode);
  void freeOperandsIfNecessary (instruction_t* instruction, operand_t* operand1, operand_t* operand2 = NULL);
  instruction_t* noFurtherInputAllowed (error_t* error,
                                 listIterator_t* tokens,
                                            int  group,
                                            int  opcode,
                                      operand_t* operand1 = NULL,
                                      operand_t* operand2 = NULL);
  void reset (void);
  void setErrorValues (error_t* error, int location);
  void setOperandLocation (token_t* token);

  REFL_DECL
};

#endif
