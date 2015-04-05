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

#ifndef ___DEBUG_PROCESSOR_INCLUDED
#define ___DEBUG_PROCESSOR_INCLUDED

#include <stdint.h>
#include "address.h"
#include "iosystem.h"
#include "memory.h"
#include "registers.h"

class processor_t : public object_t {
private:
  iosystem_t* const m_iosystem;
  memory_t* const m_memory;
  registers_t* m_registers;
  address_t* m_operandAddress;
  int64_t m_operand1;
  int64_t m_operand2;
  int m_segmentOverride;
  int m_repeat;

public:
  processor_t (memory_t* memory, iosystem_t* iosystem);
  virtual ~processor_t (void);

  virtual void clearTrap (void);
  virtual void executeNextInstruction (void);
  virtual registers_t* registers (void);
  virtual void setGeneralPurposeRegister (int index, unsigned short val);
  virtual void setInstructionPointer (unsigned short val);
  virtual void setSegmentRegister (int index, unsigned short val);
  virtual void setTrap (void);

private:
  void calculateOperandAddress (int modrmByte);
  void executeAdd (bool wide, bool carry = false);
  void executeAGI (int opcode, bool wide);
  void executeAGIAccImm (int firstByte);
  void executeAGIRegRm (int firstByte);
  void executeAGIRmImm (int firstByte);
  void executeBcdAdjust (int firstByte);
  void executeCallFarRm (int modrmByte);
  void executeCallRm (int modrmByte);
  void executeConditionalJump (int firstByte);
  void executeIncDec (int opcode, bool wide);
  void executeIncDecReg16 (int firstByte);
  void executeIncDecRm (int firstByte, int modrmByte, int opcode);
  void executeInOut (int firstByte);
  void executeInterrupt (int number);
  void executeJumpFarRm (int modrmByte);
  void executeJumpImm (int firstByte);
  void executeJumpRm (int modrmByte);
  void executeLea (void);
  void executeLesLds (int firstByte);
  void executeMisc (int firstByte);
  void executeMovAccMem (int firstByte);
  void executeMovRegImm (int firstByte);
  void executeMovRegRm (int firstByte);
  void executeMovRmImm (int firstByte);
  void executeMovSegRm (int firstByte);
  bool executeNGI (int opcode, bool wide);
  void executeNGIRm (int firstByte);
  void executePop (void);
  void executePopRm (void);
  void executePush (void);
  void executePushPopReg (int firstByte);
  void executePushPopSeg (int firstByte);
  void executePushRm (int modrmByte);
  void executeReturn (int firstByte);
  void executeReturnFar (int firstByte);
  void executeShiftInstruction (int opcode, bool wide);
  void executeShiftInstructionRm (int firstByte);
  void executeStringCompare (int step, bool wide);
  void executeStringInstruction (int firstByte);
  void executeStringLoad (int step, bool wide);
  void executeStringMove (int step, bool wide);
  void executeStringScan (int step, bool wide);
  void executeStringStore (int step, bool wide);
  void executeSub (bool wide, bool carry = false);
  void executeTestAccImm (int firstByte);
  void executeTestRegRm (int firstByte);
  void executeXchgAxReg (int firstByte);
  void executeXchgRegRm (int firstByte);
  int nextByte (void);
  int nextByteAsWord (void);
  int nextWord (void);
  bool parity (int64_t value);
  int readRegister (int regId, bool wide);
  void writeRegister (int regId, int value, bool wide);

  REFL_DECL
};

#endif
