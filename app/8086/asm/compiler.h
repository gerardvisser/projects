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

#ifndef ___DEBUG_ASM_COMPILER_INCLUDED
#define ___DEBUG_ASM_COMPILER_INCLUDED

#include "../address.h"
#include "../memory.h"
#include "instruction.h"

class compiler_t : public object_t {
private:
  memory_t* m_memory;

public:
  compiler_t (memory_t* memory);
  virtual ~compiler_t (void);

  virtual void compile (address_t* address, instruction_t* instruction);

private:
  void compileAam (address_t* address, instruction_t* instruction);
  void compileAdd (address_t* address, instruction_t* instruction);
  void compileCall (address_t* address, instruction_t* instruction);
  void compileConditionalJump (address_t* address, instruction_t* instruction);
  void compileIn (address_t* address, instruction_t* instruction);
  void compileInc (address_t* address, instruction_t* instruction);
  void compileInt (address_t* address, instruction_t* instruction);
  void compileJump (address_t* address, instruction_t* instruction);
  void compileLea (address_t* address, instruction_t* instruction);
  void compileMisc (address_t* address, instruction_t* instruction);
  void compileMov (address_t* address, instruction_t* instruction);
  void compileNot (address_t* address, instruction_t* instruction);
  void compileOut (address_t* address, instruction_t* instruction);
  void compilePush (address_t* address, instruction_t* instruction);
  void compileRet (address_t* address, instruction_t* instruction);
  void compileShift (address_t* address, instruction_t* instruction);
  void compileTest (address_t* address, instruction_t* instruction);
  void compileXchg (address_t* address, instruction_t* instruction);
  int writeDisplacement (int index, operand_t* pointer);
  int writeModrmAndDisplacement (int modrmIndex, int regOrOpCode, operand_t* regOrMemOperand);

  REFL_DECL
};

#endif
