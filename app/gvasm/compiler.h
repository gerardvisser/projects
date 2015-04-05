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

#ifndef ___GVASM__COMPILER_INCLUDED
#define ___GVASM__COMPILER_INCLUDED

#include "asmWriter.h"
#include "data.h"
#include "instruction.h"
#include "programme.h"

class compiler_t : public object_t {
public:
  compiler_t (void);
  virtual ~compiler_t (void);

  virtual void compile (asmWriter_t* outstream, programme_t* programme, int interruptNumber = 0xFFFF);

private:
  void compileAam (asmWriter_t* outstream, instruction_t* instruction);
  void compileAdd (asmWriter_t* outstream, instruction_t* instruction);
  void compileCall (asmWriter_t* outstream, instruction_t* instruction);
  void compileConditionalJump (asmWriter_t* outstream, instruction_t* instruction);
  void compileData (asmWriter_t* outstream, data_t* data);
  void compileIn (asmWriter_t* outstream, instruction_t* instruction);
  void compileInc (asmWriter_t* outstream, instruction_t* instruction);
  void compileInt (asmWriter_t* outstream, instruction_t* instruction);
  void compileJump (asmWriter_t* outstream, instruction_t* instruction);
  void compileLea (asmWriter_t* outstream, instruction_t* instruction);
  void compileMisc (asmWriter_t* outstream, instruction_t* instruction);
  void compileMov (asmWriter_t* outstream, instruction_t* instruction);
  void compileNot (asmWriter_t* outstream, instruction_t* instruction);
  void compileOut (asmWriter_t* outstream, instruction_t* instruction);
  void compilePush (asmWriter_t* outstream, instruction_t* instruction);
  void compileRet (asmWriter_t* outstream, instruction_t* instruction);
  void compileShift (asmWriter_t* outstream, instruction_t* instruction);
  void compileTest (asmWriter_t* outstream, instruction_t* instruction);
  void compileXchg (asmWriter_t* outstream, instruction_t* instruction);
  void writeDisplacement (asmWriter_t* outstream, operand_t* memOperand);
  void writeHeader (asmWriter_t* outstream, programme_t* programme, int interruptNumber);
  void writeModrm (asmWriter_t* outstream, int regOrOpCode, operand_t* regOrMemOperand);

  REFL_DECL
};

#endif
