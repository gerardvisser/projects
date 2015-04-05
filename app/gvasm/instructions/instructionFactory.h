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

#ifndef ___GVASM__INSTRUCTIONS__INSTRUCTION_FACTORY_INCLUDED
#define ___GVASM__INSTRUCTIONS__INSTRUCTION_FACTORY_INCLUDED

#include "../instruction.h"
#include "../operands/operandFactory.h"

class instructionFactory_t : public object_t {
private:
  operandFactory_t* m_operandFactory;
  pars::token_t* wm_instructionToken;

public:
  instructionFactory_t (hashMap_t* labels);
  virtual ~instructionFactory_t (void);

  virtual byteBlock_t* createInstruction (listIterator_t* tokens, pars::token_t* token, xValue_t* csOffset);
  virtual int segmentOverrideIID (void);

private:
  void assertNextIsComma (listIterator_t* tokens);
  void assertSizesAreEqual (operand_t* operand1, operand_t* operand2);
  instruction_t* createAam (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createAdd (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createCall (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createConditionalJump (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  data_t* createData (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createIn (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createInc (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createInt (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createLea (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createMov (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createNot (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createOut (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createPush (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createRet (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createShift (listIterator_t* tokens, xValue_t* csOffset, int instrId);
  instruction_t* createXchg (listIterator_t* tokens, xValue_t* csOffset, int instrId);

  REFL_DECL
};

#endif
