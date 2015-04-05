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
#include "asm/instructionCodes.h"
#include "processor.h"

REFL_IMPL (processor_t, object_t)

#include <stdio.h>
#define NOT_YET_IMPLEMENTED printf ("\x1B[31mNot yet implemented.\x1B[0m\n")

#define I_DIVIDE_OVERFLOW 0
#define I_TRAP            1
#define I_BREAKPOINT      3
#define I_OVERFLOW        4
#define I_INVALID_OPCODE  6
#define I_NO_COPROCESSOR  7

#define FLAGS_MASK (F_CARRY | F_PARITY | F_ADJUST | F_ZERO | F_SIGN | F_TRAP | F_INTERRUPT | F_DIRECTION | F_OVERFLOW)
#define STATUS_FLAGS_CLEAR_MASK ~(F_OVERFLOW | F_SIGN | F_ZERO | F_ADJUST | F_PARITY | F_CARRY)
#define ROTATE_FLAGS_CLEAR_MASK ~(F_OVERFLOW | F_CARRY)

#define flag(id) (m_registers->m_flags & id)
#define extendSign(value, wide) if ((value & signMask[wide]) != 0) value |= signExtension[wide]
#define instructionHasMemoryOperand(modrmByte) (modrmByte >> 6 != 3)

static const int64_t carryMask[] = {0x100, 0x10000};
static const int64_t signMask[] = {0x80, 0x8000, 0x80000000};
static const int64_t valueMask[] = {0xFF, 0xFFFF};
static const int64_t signedMax[] = {0x7F, 0x7FFF};
static const int64_t signedMin[] = {-0x80, -0x8000};
static const int64_t signExtension[] = {0xFFFFFFFFFFFFFF00, 0xFFFFFFFFFFFF0000, 0xFFFFFFFF00000000};
static const int64_t operandWidth[] = {8, 16};

processor_t::processor_t (memory_t* memory, iosystem_t* iosystem) : m_memory (memory), m_iosystem (iosystem) {
  ___CBTPUSH;

  memory->reserve ();
  iosystem->reserve ();
  m_registers = new registers_t ();
  m_operandAddress = new address_t ();
  m_segmentOverride = -1;
  m_repeat = -1;

  for (int i = 0; i < 256; ++i) {
    memory->writeWord (4*i, 0);
    memory->writeWord (4*i + 2, 0x50);
  }
  memory->writeByte (0x500, OPC_IRET);

  ___CBTPOP;
}

processor_t::~processor_t (void) {
  ___CBTPUSH;

  m_operandAddress->dump ();
  m_registers->dump ();
  m_iosystem->dump ();
  m_memory->dump ();

  ___CBTPOP;
}

void processor_t::calculateOperandAddress (int modrmByte) {
  ___CBTPUSH;

  int offset;
  switch (modrmByte & 7) {
  case 0:
    m_operandAddress->setSegment (m_registers->m_seg[DS]);
    offset = m_registers->m_gen[BX] + m_registers->m_gen[SI];
    break;
  case 1:
    m_operandAddress->setSegment (m_registers->m_seg[DS]);
    offset = m_registers->m_gen[BX] + m_registers->m_gen[DI];
    break;
  case 2:
    m_operandAddress->setSegment (m_registers->m_seg[SS]);
    offset = m_registers->m_gen[BP] + m_registers->m_gen[SI];
    break;
  case 3:
    m_operandAddress->setSegment (m_registers->m_seg[SS]);
    offset = m_registers->m_gen[BP] + m_registers->m_gen[DI];
    break;
  case 4:
    m_operandAddress->setSegment (m_registers->m_seg[DS]);
    offset = m_registers->m_gen[SI];
    break;
  case 5:
    m_operandAddress->setSegment (m_registers->m_seg[DS]);
    offset = m_registers->m_gen[DI];
    break;
  case 6:
    m_operandAddress->setSegment (m_registers->m_seg[SS]);
    offset = m_registers->m_gen[BP];
    break;
  case 7:
    m_operandAddress->setSegment (m_registers->m_seg[DS]);
    offset = m_registers->m_gen[BX];
  }

  switch (modrmByte >> 6) {
  case 0:
    if ((modrmByte & 7) == 6) {
      m_operandAddress->setSegment (m_registers->m_seg[DS]);
      offset = nextWord ();
    }
    break;
  case 1:
    offset += nextByteAsWord ();
    break;
  case 2:
    offset += nextWord ();
    break;
  }
  m_operandAddress->setOffset (offset);

  if (m_segmentOverride != -1) {
    m_operandAddress->setSegment (m_registers->m_seg[m_segmentOverride]);
  }

  ___CBTPOP;
}

void processor_t::clearTrap (void) {
  ___CBTPUSH;

  m_registers->m_flags &= ~F_TRAP;

  ___CBTPOP;
}

void processor_t::executeAdd (bool wide, bool carry) {
  ___CBTPUSH;

  int64_t x, y;
  int64_t u = m_operand1 & 0xF;
  int64_t v = m_operand2 & 0xF;

  if (wide) {
    x = (int16_t) m_operand1;
    y = (int16_t) m_operand2;
  } else {
    x = (int8_t) m_operand1;
    y = (int8_t) m_operand2;
  }

  m_operand1 += m_operand2 + carry;
  x += y + carry;

  if ((u + v + carry & 0x10) != 0) m_registers->m_flags |= F_ADJUST;
  if ((m_operand1 & carryMask[wide]) != 0) m_registers->m_flags |= F_CARRY;
  if (x < signedMin[wide] || x > signedMax[wide]) m_registers->m_flags |= F_OVERFLOW;
  m_operand1 &= valueMask[wide];

  ___CBTPOP;
}

/* TODO: Check het zetten van de vlaggen. */
void processor_t::executeAGI (int opcode, bool wide) {
  ___CBTPUSH;

  bool carry = flag (F_CARRY) != 0;
  m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK;

  int64_t result;
  switch (opcode) {
  case OPC_ADD: 
    executeAdd (wide);
    result = m_operand1;
    break;

  case OPC_OR:
    m_operand1 |= m_operand2;
    result = m_operand1;
    break;

  case OPC_ADC:
    executeAdd (wide, carry);
    result = m_operand1;
    break;

  case OPC_SBB:
    executeSub (wide, carry);
    result = m_operand1;
    break;

  case OPC_AND:
    m_operand1 &= m_operand2;
    result = m_operand1;
    break;

  case OPC_SUB:
    executeSub (wide);
    result = m_operand1;
    break;

  case OPC_XOR:
    m_operand1 ^= m_operand2;
    result = m_operand1;
    break;

  case OPC_CMP: {
    int64_t oldValue = m_operand1;
    executeSub (wide);
    result = m_operand1;
    m_operand1 = oldValue;
  }
  }

  if ((result & signMask[wide]) != 0) m_registers->m_flags |= F_SIGN;
  if (result == 0) m_registers->m_flags |= F_ZERO;
  if (parity (result)) m_registers->m_flags |= F_PARITY;

  ___CBTPOP;
}

void processor_t::executeAGIAccImm (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  const int opcode = firstByte >> 3 & 7;
  m_operand1 = readRegister (0, wide);
  m_operand2 = wide ? nextWord () : nextByte ();
  executeAGI (opcode, wide);
  writeRegister (0, m_operand1, wide);

  ___CBTPOP;
}

void processor_t::executeAGIRegRm (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  const int modrmByte = nextByte ();
  const int opcode = firstByte >> 3 & 7;
  const bool dstRrr = (firstByte & 2) != 0;

  if (dstRrr) {
    int dstRegId = modrmByte >> 3 & 7;
    m_operand1 = readRegister (dstRegId, wide);
    if (instructionHasMemoryOperand (modrmByte)) {
      calculateOperandAddress (modrmByte);
      m_operand2 = m_memory->read (m_operandAddress->value (), wide);
    } else {
      m_operand2 = readRegister (modrmByte & 7, wide);
    }
    executeAGI (opcode, wide);
    writeRegister (dstRegId, m_operand1, wide);
  } else {
    m_operand2 = readRegister (modrmByte >> 3 & 7, wide);
    if (instructionHasMemoryOperand (modrmByte)) {
      calculateOperandAddress (modrmByte);
      int operandAddress = m_operandAddress->value ();
      m_operand1 = m_memory->read (operandAddress, wide);
      executeAGI (opcode, wide);
      m_memory->write (operandAddress, m_operand1, wide);
    } else {
      int dstRegId = modrmByte & 7;
      m_operand1 = readRegister (dstRegId, wide);
      executeAGI (opcode, wide);
      writeRegister (dstRegId, m_operand1, wide);
    }
  }

  ___CBTPOP;
}

void processor_t::executeAGIRmImm (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  const int modrmByte = nextByte ();
  const int opcode = modrmByte >> 3 & 7;
  const bool wideImm = (firstByte & 2) == 0;

  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    int operandAddress = m_operandAddress->value ();
    m_operand1 = m_memory->read (operandAddress, wide);
    m_operand2 = wide ? wideImm ? nextWord () : nextByteAsWord () : nextByte ();
    executeAGI (opcode, wide);
    m_memory->write (operandAddress, m_operand1, wide);
  } else {
    int dstRegId = modrmByte & 7;
    m_operand1 = readRegister (dstRegId, wide);
    m_operand2 = wide ? wideImm ? nextWord () : nextByteAsWord () : nextByte ();
    executeAGI (opcode, wide);
    writeRegister (dstRegId, m_operand1, wide);
  }

  ___CBTPOP;
}

void processor_t::executeBcdAdjust (int firstByte) {
  ___CBTPUSH;

  bool carry = flag (F_CARRY) != 0;
  bool adjust = flag (F_ADJUST) != 0;
  m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK;

  switch (firstByte) {
  case OPC_DAA:
    m_operand1 = readRegister (0, false);
    if ((m_operand1 & 0xF) > 9 || adjust) {
      m_registers->m_flags |= F_ADJUST;
      m_operand1 += 6;
    }
    if (m_operand1 > 0x9F || carry) {
      m_registers->m_flags |= F_CARRY;
      m_operand1 += 0x60;
    }
    writeRegister (0, m_operand1, false);
    break;

  case OPC_DAS: /* TODO: TEST. */
    m_operand1 = readRegister (0, false);
    if ((m_operand1 & 0xF) > 9 || adjust) {
      m_registers->m_flags |= F_ADJUST;
      m_operand1 -= 6;
    }
    if (m_operand1 > 0x9F || carry) {
      m_registers->m_flags |= F_CARRY;
      m_operand1 -= 0x60;
    }
    writeRegister (0, m_operand1, false);
    break;

  case OPC_AAA:
    m_operand1 = m_registers->m_gen[AX];
    if ((m_operand1 & 0xF) > 9 || adjust) {
      m_registers->m_flags |= F_ADJUST | F_CARRY;
      m_operand1 &= 0xFF0F; /* According to some source this step is present on 8086/8088 and removed from later models. */
      m_operand1 += 0x106;
    }
    m_operand1 &= 0xFF0F;
    m_registers->m_gen[AX] = m_operand1;
    break;

  case OPC_AAS: /* TODO: TEST. */
    m_operand1 = m_registers->m_gen[AX];
    if ((m_operand1 & 0xF) > 9 || adjust) {
      m_registers->m_flags |= F_ADJUST | F_CARRY;
      m_operand1 -= 0x106;
    }
    m_operand1 &= 0xFF0F;
    m_registers->m_gen[AX] = m_operand1;
    break;
  }
  if ((m_operand1 & 0x80) != 0) m_registers->m_flags |= F_SIGN;
  if ((m_operand1 & 0xFF) == 0) m_registers->m_flags |= F_ZERO;
  if (parity (m_operand1)) m_registers->m_flags |= F_PARITY;

  ___CBTPOP;
}

void processor_t::executeCallFarRm (int modrmByte) {
  ___CBTPUSH;

  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    int operandAddress = m_operandAddress->value ();
    m_operand1 = m_registers->m_seg[CS];
    executePush ();
    m_operand1 = m_registers->m_ip;
    executePush ();
    m_registers->m_ip = m_memory->readWord (operandAddress);
    m_registers->m_seg[CS] = m_memory->readWord (operandAddress + 2);
  } else {
    executeInterrupt (I_INVALID_OPCODE);
  }

  ___CBTPOP;
}

void processor_t::executeCallRm (int modrmByte) {
  ___CBTPUSH;

  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    m_operand1 = m_registers->m_ip;
    m_registers->m_ip = m_memory->readWord (m_operandAddress->value ());
  } else {
    int regId = modrmByte & 7;
    m_operand1 = m_registers->m_ip;
    m_registers->m_ip = m_registers->m_gen[regId];
  }
  executePush ();

  ___CBTPOP;
}

void processor_t::executeConditionalJump (int firstByte) {
  ___CBTPUSH;

  bool conditionHolds;
  int displacement = nextByteAsWord ();

  /* TODO: check these */
  switch (firstByte) {
  case OPC_JO: conditionHolds = flag (F_OVERFLOW) != 0; break;
  case OPC_JNO: conditionHolds = flag (F_OVERFLOW) == 0; break;
  case OPC_JB: conditionHolds = flag (F_CARRY) != 0; break;
  case OPC_JNB: conditionHolds = flag (F_CARRY) == 0; break;
  case OPC_JZ: conditionHolds = flag (F_ZERO) != 0; break;
  case OPC_JNZ: conditionHolds = flag (F_ZERO) == 0; break;
  case OPC_JBE: conditionHolds = flag (F_CARRY) != 0 || flag (F_ZERO) != 0; break;
  case OPC_JA: conditionHolds = flag (F_CARRY) == 0 && flag (F_ZERO) == 0; break;
  case OPC_JS: conditionHolds = flag (F_SIGN) != 0; break;
  case OPC_JNS: conditionHolds = flag (F_SIGN) == 0; break;
  case OPC_JPE: conditionHolds = flag (F_PARITY) != 0; break;
  case OPC_JPO: conditionHolds = flag (F_PARITY) == 0; break;
  case OPC_JL: conditionHolds = flag (F_SIGN) != 0 ^ flag (F_OVERFLOW) != 0; break;
  case OPC_JGE: conditionHolds = (flag (F_SIGN) != 0) == (flag (F_OVERFLOW) != 0); break;
  case OPC_JLE: conditionHolds = flag (F_ZERO) != 0 || (flag (F_SIGN) != 0 ^ flag (F_OVERFLOW) != 0); break;
  case OPC_JG: conditionHolds = flag (F_ZERO) == 0 && (flag (F_SIGN) != 0) == (flag (F_OVERFLOW) != 0); break;
  case OPC_LOOPNZ:
    --m_registers->m_gen[CX];
    conditionHolds = flag (F_ZERO) == 0 && m_registers->m_gen[CX] != 0;
    break;
  case OPC_LOOPZ:
    --m_registers->m_gen[CX];
    conditionHolds = flag (F_ZERO) != 0 && m_registers->m_gen[CX] != 0;
    break;
  case OPC_LOOP:
    --m_registers->m_gen[CX];
    conditionHolds = m_registers->m_gen[CX] != 0;
    break;
  case OPC_JCXZ: conditionHolds = m_registers->m_gen[CX] == 0;
  }

  if (conditionHolds) {
    m_registers->m_ip += displacement;
  }

  ___CBTPOP;
}

void processor_t::executeIncDec (int opcode, bool wide) {
  ___CBTPUSH;

  m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK | F_CARRY;
  if (opcode == OPC_INC) {
    if ((m_operand1 & 0xF) == 0xF) m_registers->m_flags |= F_ADJUST;
    if (m_operand1 == signedMax[wide]) m_registers->m_flags |= F_OVERFLOW;
    ++m_operand1;
  } else {
    if ((m_operand1 & 0xF) == 0) m_registers->m_flags |= F_ADJUST;
    if (m_operand1 == signMask[wide]) m_registers->m_flags |= F_OVERFLOW;
    --m_operand1;
  }
  m_operand1 &= valueMask[wide];

  if ((m_operand1 & signMask[wide]) != 0) m_registers->m_flags |= F_SIGN;
  if (m_operand1 == 0) m_registers->m_flags |= F_ZERO;
  if (parity (m_operand1)) m_registers->m_flags |= F_PARITY;

  ___CBTPOP;
}

void processor_t::executeIncDecReg16 (int firstByte) {
  ___CBTPUSH;

  const int regId = firstByte & 7;
  const int opcode = firstByte >> 3 & 7;
  m_operand1 = m_registers->m_gen[regId];
  executeIncDec (opcode, true);
  m_registers->m_gen[regId] = m_operand1;

  ___CBTPOP;
}

void processor_t::executeIncDecRm (int firstByte, int modrmByte, int opcode) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;

  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    int operandAddress = m_operandAddress->value ();
    m_operand1 = m_memory->read (operandAddress, wide);
    executeIncDec (opcode, wide);
    m_memory->write (operandAddress, m_operand1, wide);
  } else {
    int regId = modrmByte & 7;
    m_operand1 = readRegister (regId, wide);
    executeIncDec (opcode, wide);
    writeRegister (regId, m_operand1, wide);
  }

  ___CBTPOP;
}

void processor_t::executeInOut (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  int port = (firstByte & 8) != 0 ? m_registers->m_gen[DX] : nextByte ();
  if ((firstByte & 2) != 0) {
    if (wide) {
      m_iosystem->writeWord (port, m_registers->m_gen[AX]);
    } else {
      m_iosystem->writeByte (port, m_registers->m_gen[AX]);
    }
  } else {
    if (wide) {
      m_operand1 = m_iosystem->readWord (port);
    } else {
      m_operand1 = m_iosystem->readByte (port);
    }
    writeRegister (AX, m_operand1, wide);
  }

  ___CBTPOP;
}

/* 0 <= number < 256 */
void processor_t::executeInterrupt (int number) {
  ___CBTPUSH;

  int interruptTableOffset = number << 2;
  m_operand1 = m_registers->m_flags;
  executePush ();
  m_operand1 = m_registers->m_seg[CS];
  executePush ();
  m_operand1 = m_registers->m_ip;
  executePush ();
  m_registers->m_ip = m_memory->readWord (interruptTableOffset);
  m_registers->m_seg[CS] = m_memory->readWord (interruptTableOffset + 2);

  ___CBTPOP;
}

void processor_t::executeJumpFarRm (int modrmByte) {
  ___CBTPUSH;

  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    int operandAddress = m_operandAddress->value ();
    m_registers->m_ip = m_memory->readWord (operandAddress);
    m_registers->m_seg[CS] = m_memory->readWord (operandAddress + 2);
  } else {
    executeInterrupt (I_INVALID_OPCODE);
  }

  ___CBTPOP;
}

void processor_t::executeJumpImm (int firstByte) {
  ___CBTPUSH;

  int displacement, cs, ip;

  switch (firstByte) {
  case 0xE8:
    displacement = nextWord ();
    m_operand1 = m_registers->m_ip;
    executePush ();
    m_registers->m_ip += displacement;
    break;

  case 0xE9:
    displacement = nextWord ();
    m_registers->m_ip += displacement;
    break;

  case 0xEA:
    ip = nextWord ();
    cs = nextWord ();
    m_registers->m_ip = ip;
    m_registers->m_seg[CS] = cs;
    break;

  case 0xEB:
    displacement = nextByteAsWord ();
    m_registers->m_ip += displacement;
    break;
  }

  ___CBTPOP;
}

void processor_t::executeJumpRm (int modrmByte) {
  ___CBTPUSH;

  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    m_registers->m_ip = m_memory->readWord (m_operandAddress->value ());
  } else {
    int regId = modrmByte & 7;
    m_registers->m_ip = m_registers->m_gen[regId];
  }

  ___CBTPOP;
}

void processor_t::executeLea (void) {
  ___CBTPUSH;

  const int modrmByte = nextByte ();
  const int dstRegId = modrmByte >> 3 & 7;

  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    m_registers->m_gen[dstRegId] = m_operandAddress->offset ();
  } else {
    executeInterrupt (I_INVALID_OPCODE);
  }

  ___CBTPOP;
}

void processor_t::executeLesLds (int firstByte) {
  ___CBTPUSH;

  const int modrmByte = nextByte ();
  const int dstRegId = modrmByte >> 3 & 7;
  const int dstSegId = firstByte & 1 ? DS : ES;

  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    int operandAddress = m_operandAddress->value ();
    m_registers->m_gen[dstRegId] = m_memory->readWord (operandAddress);
    m_registers->m_seg[dstSegId] = m_memory->readWord (operandAddress + 2);
  } else {
    executeInterrupt (I_INVALID_OPCODE);
  }

  ___CBTPOP;
}

void processor_t::executeMisc (int firstByte) {
  ___CBTPUSH;

  switch (firstByte) {
  case OPC_CBW:
    m_registers->m_gen[AX] &= 0xFF;
    if ((m_registers->m_gen[AX] & 0x80) != 0) {
      m_registers->m_gen[AX] |= 0xFF00;
    }
    break;

  case OPC_CWD:
    if ((m_registers->m_gen[AX] & 0x8000) != 0) {
      m_registers->m_gen[DX] = 0xFFFF;
    } else {
      m_registers->m_gen[DX] = 0;
    }
    break;

  case 0x9A: {
    int ip = nextWord ();
    int cs = nextWord ();
    m_operand1 = m_registers->m_seg[CS];
    executePush ();
    m_operand1 = m_registers->m_ip;
    executePush ();
    m_registers->m_ip = ip;
    m_registers->m_seg[CS] = cs;
  } break;

  case OPC_WAIT:
    /* No 8087 to wait for; test-pin always low. */
    break;

  case OPC_PUSHF:
    m_operand1 = m_registers->m_flags;
    executePush ();
    break;

  case OPC_POPF:
    executePop ();
    m_registers->m_flags &= ~FLAGS_MASK;
    m_registers->m_flags |= m_operand1 & FLAGS_MASK;
    break;

  case OPC_SAHF:
    m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK | F_OVERFLOW;
    m_operand1 = readRegister (4, false);
    m_registers->m_flags |= m_operand1 & FLAGS_MASK;
    break;

  case OPC_LAHF:
    writeRegister (4, m_registers->m_flags, false);
    break;

  case 0xCC:
    executeInterrupt (I_BREAKPOINT);
    break;

  case 0xCD:
    executeInterrupt (nextByte ());
    break;

  case OPC_INTO:
    if (flag (F_OVERFLOW) != 0) {
      executeInterrupt (I_OVERFLOW);
    }
    break;

  case OPC_IRET:
    executePop ();
    m_registers->m_ip = m_operand1;
    executePop ();
    m_registers->m_seg[CS] = m_operand1;
    executePop ();
    m_registers->m_flags &= ~FLAGS_MASK;
    m_registers->m_flags |= m_operand1 & FLAGS_MASK;
    break;

  case OPC_AAM:
    m_operand2 = nextByte ();
    if (m_operand2 != 0) {
      m_operand1 = m_registers->m_gen[AX] & 0xFF;
      m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK;
      int ah = m_operand1 / m_operand2;
      int al = m_operand1 % m_operand2;
      m_registers->m_gen[AX] = ah << 8;
      m_registers->m_gen[AX] |= al;
      if (al == 0) m_registers->m_flags |= F_ZERO;
      if (parity (al)) m_registers->m_flags |= F_PARITY;
      if ((al & 0x80) != 0) m_registers->m_flags |= F_SIGN;
    } else {
      executeInterrupt (I_DIVIDE_OVERFLOW);
    }
    break;

  case OPC_AAD: /* TODO: TEST. */
    m_operand2 = nextByte ();
    m_operand1 = m_registers->m_gen[AX] >> 8;
    m_operand1 *= m_operand2;
    m_operand1 += m_registers->m_gen[AX] & 0xFF;
    m_operand1 &= 0xFF;
    m_registers->m_gen[AX] = m_operand1;
    m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK;
    if (m_operand1 == 0) m_registers->m_flags |= F_ZERO;
    if (parity (m_operand1)) m_registers->m_flags |= F_PARITY;
    if ((m_operand1 & 0x80) != 0) m_registers->m_flags |= F_SIGN;
    break;

  case 0xD6:
    executeInterrupt (I_INVALID_OPCODE);
    break;

  case OPC_XLAT:
    if (m_segmentOverride != -1) {
      m_operandAddress->setSegment (m_registers->m_seg[m_segmentOverride]);
    } else {
      m_operandAddress->setSegment (m_registers->m_seg[DS]);
    }
    m_operandAddress->setOffset (m_registers->m_gen[BX] + readRegister (0, false));
    writeRegister (0, m_memory->readByte (m_operandAddress->value ()), false);
    break;

  case OPC_LOCK:
    /* Of use in a multiprocessor environment: not implemented here. */
    break;

  case 0xF1:
    executeInterrupt (I_INVALID_OPCODE);
    break;

  case OPC_REPNZ:
    m_repeat = OPC_REPNZ;
    executeNextInstruction ();
    m_repeat = -1;
    break;

  case OPC_REPZ:
    m_repeat = OPC_REPZ;
    executeNextInstruction ();
    m_repeat = -1;
    break;

  case OPC_HLT:
    /* Halts the CPU until the next external interrupt.
       TODO: to be implemented. */
    NOT_YET_IMPLEMENTED;
    break;

  case OPC_CMC:
    m_registers->m_flags ^= F_CARRY;
    break;

  case OPC_CLC:
    m_registers->m_flags &= ~F_CARRY;
    break;

  case OPC_STC:
    m_registers->m_flags |= F_CARRY;
    break;

  case OPC_CLI:
    m_registers->m_flags &= ~F_INTERRUPT;
    break;

  case OPC_STI:
    m_registers->m_flags |= F_INTERRUPT;
    break;

  case OPC_CLD:
    m_registers->m_flags &= ~F_DIRECTION;
    break;

  case OPC_STD:
    m_registers->m_flags |= F_DIRECTION;
    break;
  }

  ___CBTPOP;
}

void processor_t::executeMovAccMem (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  const bool dstMem = (firstByte & 2) != 0;

  m_operandAddress->setOffset (nextWord ());
  if (m_segmentOverride != -1) {
    m_operandAddress->setSegment (m_registers->m_seg[m_segmentOverride]);
  } else {
    m_operandAddress->setSegment (m_registers->m_seg[DS]);
  }
  if (dstMem) {
    m_memory->write (m_operandAddress->value (), m_registers->m_gen[AX], wide);
  } else {
    m_operand1 = m_memory->read (m_operandAddress->value (), wide);
    writeRegister (AX, m_operand1, wide);
  }

  ___CBTPOP;
}

void processor_t::executeMovRegImm (int firstByte) {
  ___CBTPUSH;

  const int regId = firstByte & 7;
  const bool wide = (firstByte & 8) != 0;
  if (wide) {
    m_registers->m_gen[regId] = nextWord ();
  } else {
    int value = nextByte ();
    writeRegister (regId, value, false);
  }

  ___CBTPOP;
}

void processor_t::executeMovRegRm (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  const int modrmByte = nextByte ();
  const bool dstRrr = (firstByte & 2) != 0;
  const int rrrId = modrmByte >> 3 & 7;

  if (dstRrr) {
    if (instructionHasMemoryOperand (modrmByte)) {
      calculateOperandAddress (modrmByte);
      m_operand1 = m_memory->read (m_operandAddress->value (), wide);
    } else {
      m_operand1 = readRegister (modrmByte & 7, wide);
    }
    writeRegister (rrrId, m_operand1, wide);
  } else {
    m_operand1 = readRegister (rrrId, wide);
    if (instructionHasMemoryOperand (modrmByte)) {
      calculateOperandAddress (modrmByte);
      m_memory->write (m_operandAddress->value (), m_operand1, wide);
    } else {
      writeRegister (modrmByte & 7, m_operand1, wide);
    }
  }

  ___CBTPOP;
}

void processor_t::executeMovRmImm (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  const int modrmByte = nextByte ();
  const int opcode = modrmByte >> 3 & 7;

  if (opcode == 0) {
    if (instructionHasMemoryOperand (modrmByte)) {
      calculateOperandAddress (modrmByte);
      m_operand1 = wide ? nextWord () : nextByte ();
      m_memory->write (m_operandAddress->value (), m_operand1, wide);
    } else {
      int dstRegId = modrmByte & 7;
      m_operand1 = wide ? nextWord () : nextByte ();
      writeRegister (dstRegId, m_operand1, wide);
    }
  } else {
    executeInterrupt (I_INVALID_OPCODE);
  }

  ___CBTPOP;
}

void processor_t::executeMovSegRm (int firstByte) {
  ___CBTPUSH;

  const int modrmByte = nextByte ();
  const bool dstSeg = (firstByte & 2) != 0;
  const int segId = modrmByte >> 3 & 3;

  if (dstSeg) {
    if (instructionHasMemoryOperand (modrmByte)) {
      calculateOperandAddress (modrmByte);
      m_registers->m_seg[segId] = m_memory->readWord (m_operandAddress->value ());
    } else {
      m_registers->m_seg[segId] = m_registers->m_gen[modrmByte & 7];
    }
  } else if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    m_memory->writeWord (m_operandAddress->value (), m_registers->m_seg[segId]);
  } else {
    m_registers->m_gen[modrmByte & 7] = m_registers->m_seg[segId];
  }

  ___CBTPOP;
}

void processor_t::executeNextInstruction (void) {
  ___CBTPUSH;

  int firstByte = nextByte ();

  switch (firstByte >> 6) {
  case 0: /* 00 - 3F */
    switch (firstByte & 6) {
    case 6:
      switch (firstByte & 0x21) {
      case 0:
      case 1: executePushPopSeg (firstByte); break;

      case 0x20:
        m_segmentOverride = firstByte >> 3 & 3;
        executeNextInstruction ();
        m_segmentOverride = -1;
        break;

      case 0x21: executeBcdAdjust (firstByte);
      }
      break;

    case 4: executeAGIAccImm (firstByte); break;
    default: executeAGIRegRm (firstByte);
    }
    break;

  case 1: /* 40 - 7F */
    switch (firstByte & 0x30) {
    case 0: /* 40 - 4F */ executeIncDecReg16 (firstByte); break;
    case 0x10: /* 50 - 5F */ executePushPopReg (firstByte); break;
    case 0x20: /* 60 - 6F: Undefined. */ executeInterrupt (I_INVALID_OPCODE); break;
    case 0x30: /* 70 - 7F */ executeConditionalJump (firstByte); break;
    }
    break;

  case 2: /* 80 - BF */
    switch (firstByte & 0x30) {
    case 0: /* 80 - 8F */
      switch (firstByte & 0xC) {
      case 0: executeAGIRmImm (firstByte); break;
      case 8: executeMovRegRm (firstByte); break;
      case 4:
        if ((firstByte & 2) != 0) {
          executeXchgRegRm (firstByte);
        } else {
          executeTestRegRm (firstByte);
        }
        break;

      default:
        switch (firstByte & 3) {
        case 0:
        case 2: executeMovSegRm (firstByte); break;
        case 3: executePopRm (); break;
        case 1: executeLea (); break;
        }
      }
      break;

    case 0x10: /* 90 - 9F */
      if (firstByte < 0x98) {
        executeXchgAxReg (firstByte);
      } else {
        executeMisc (firstByte);
      }
      break;

    case 0x20: /* A0 - AF */
      switch (firstByte & 0x0E) {
      case 0:
      case 2: executeMovAccMem (firstByte); break;
      case 8: executeTestAccImm (firstByte); break;
      default: executeStringInstruction (firstByte);
      }
      break;

    case 0x30: /* B0 - BF */ executeMovRegImm (firstByte); break;
    }
    break;

  case 3: /* C0 - FF */
    switch (firstByte & 0x30) {
    case 0: /* C0 - CF */
      switch (firstByte & 0x0E) {
      case 0:
      case 8: executeInterrupt (I_INVALID_OPCODE); break;
      case 2: executeReturn (firstByte); break;
      case 4: executeLesLds (firstByte); break;
      case 6: executeMovRmImm (firstByte); break;
      case 10: executeReturnFar (firstByte); break;
      default: executeMisc (firstByte);
      }
      break;

    case 0x10: /* D0 - DF */
      switch (firstByte & 0x0C) {
      case 0: executeShiftInstructionRm (firstByte); break;
      case 4: executeMisc (firstByte); break;
      default: executeInterrupt (I_NO_COPROCESSOR);
      }
      break;

    case 0x20: /* E0 - EF */
      switch (firstByte & 0x0C) {
      case 0: executeConditionalJump (firstByte); break;
      case 8: executeJumpImm (firstByte); break;
      default: executeInOut (firstByte);
      }
      break;

    case 0x30: /* F0 - FF */
      switch (firstByte & 0x0E) {
      case 6: executeNGIRm (firstByte); break;

      case 14: {
        const int modrmByte = nextByte ();
        const int opcode = modrmByte >> 3 & 7;
        if (opcode < 2) {
          executeIncDecRm (firstByte, modrmByte, opcode);
        } else if (opcode < 7) {
          if (firstByte == 0xFF) {
            switch (opcode) {
            case OPC_CALL: executeCallRm (modrmByte); break;
            case OPC_CALLF: executeCallFarRm (modrmByte); break;
            case OPC_JMP: executeJumpRm (modrmByte); break;
            case OPC_JMPF: executeJumpFarRm (modrmByte); break;
            case 6: executePushRm (modrmByte);
            }
          } else {
            executeInterrupt (I_INVALID_OPCODE);
          }
        } else {
          executeInterrupt (I_INVALID_OPCODE);
        }
      } break;

      default: executeMisc (firstByte);
      }
    }
  }

  ___CBTPOP;
}

/* TODO: Check het zetten van de vlaggen. */
bool processor_t::executeNGI (int opcode, bool wide) {
  ___CBTPUSH;

  bool operandToBeWritten;
  switch (opcode) {
  case OPC_TEST:
    operandToBeWritten = false;
    m_operand2 = wide ? nextWord () : nextByte ();
    executeAGI (OPC_AND, wide);
    break;

  case OPC_NOT:
    operandToBeWritten = true;
    m_operand1 = ~m_operand1;
    break;

  case OPC_NEG:
    operandToBeWritten = true;
    m_operand2 = m_operand1;
    m_operand1 = 0;
    executeAGI (OPC_SUB, wide); /* Doing it like this takes care of flags. */
    break;

  case OPC_MUL:
    operandToBeWritten = false;
    m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK;
    m_operand2 = m_registers->m_gen[AX] & valueMask[wide];
    m_operand2 *= m_operand1;
    m_registers->m_gen[AX] = m_operand2;
    if (wide) {
      m_registers->m_gen[DX] = m_operand2 >> 16;
    }
    if (m_operand2 > valueMask[wide]) {
      m_registers->m_flags |= F_OVERFLOW;
      m_registers->m_flags |= F_CARRY;
    }
    if ((m_operand2 & signMask[wide]) != 0) m_registers->m_flags |= F_SIGN;
    if ((m_operand2 & valueMask[wide]) == 0) m_registers->m_flags |= F_ZERO;
    if (parity (m_operand2)) m_registers->m_flags |= F_PARITY;
    break;

  case OPC_IMUL:
    operandToBeWritten = false;
    m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK;
    m_operand2 = m_registers->m_gen[AX] & valueMask[wide];
    extendSign (m_operand1, wide);
    extendSign (m_operand2, wide);
    m_operand2 *= m_operand1;
    m_registers->m_gen[AX] = m_operand2;
    if (wide) {
      m_registers->m_gen[DX] = m_operand2 >> 16;
    }
    if (m_operand2 < signedMin[wide] || m_operand2 > signedMax[wide]) {
      m_registers->m_flags |= F_OVERFLOW;
      m_registers->m_flags |= F_CARRY;
    }
    if ((m_operand2 & signMask[wide]) != 0) m_registers->m_flags |= F_SIGN;
    if ((m_operand2 & valueMask[wide]) == 0) m_registers->m_flags |= F_ZERO;
    if (parity (m_operand2)) m_registers->m_flags |= F_PARITY;
    break;

  case OPC_DIV:
    operandToBeWritten = false;
    if (m_operand1 != 0) {
      m_operand2 = wide ? m_registers->m_gen[DX] : 0;
      m_operand2 <<= 16;
      m_operand2 |= m_registers->m_gen[AX];
      int64_t remainder = m_operand2 % m_operand1;
      m_operand2 /= m_operand1;
      if (m_operand2 <= valueMask[wide]) {
        if (wide) {
          m_registers->m_gen[DX] = remainder;
        } else {
          m_operand2 |= remainder << 8;
        }
        m_registers->m_gen[AX] = m_operand2;
      } else {
        executeInterrupt (I_DIVIDE_OVERFLOW);
      }
    } else {
      executeInterrupt (I_DIVIDE_OVERFLOW);
    }
    break;

  case OPC_IDIV:
    operandToBeWritten = false;
    if (m_operand1 != 0) {
      m_operand2 = wide ? m_registers->m_gen[DX] : 0;
      m_operand2 <<= 16;
      m_operand2 |= m_registers->m_gen[AX];
      extendSign (m_operand2, wide + 1);
      extendSign (m_operand1, wide);
      int64_t remainder = m_operand2 % m_operand1;
      m_operand2 /= m_operand1;
      if (m_operand2 >= signedMin[wide] && m_operand2 <= signedMax[wide]) {
        if (wide) {
          m_registers->m_gen[DX] = remainder;
        } else {
          m_operand2 &= 0xFF;
          m_operand2 |= remainder << 8;
        }
        m_registers->m_gen[AX] = m_operand2;
      } else {
        executeInterrupt (I_DIVIDE_OVERFLOW);
      }
    } else {
      executeInterrupt (I_DIVIDE_OVERFLOW);
    }
    break;

  default: executeInterrupt (I_INVALID_OPCODE);
  }

  ___CBTPOP;
  return operandToBeWritten;
}

void processor_t::executeNGIRm (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  const int modrmByte = nextByte ();
  const int opcode = modrmByte >> 3 & 7;
  bool operandToBeWritten;

  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    int operandAddress = m_operandAddress->value ();
    m_operand1 = m_memory->read (operandAddress, wide);
    operandToBeWritten = executeNGI (opcode, wide);
    if (operandToBeWritten) {
      m_memory->write (operandAddress, m_operand1, wide);
    }
  } else {
    int regId = modrmByte & 7;
    m_operand1 = readRegister (regId, wide);
    operandToBeWritten = executeNGI (opcode, wide);
    if (operandToBeWritten) {
      writeRegister (regId, m_operand1, wide);
    }
  }

  ___CBTPOP;
}

void processor_t::executePop (void) {
  ___CBTPUSH;

  m_operand1 = m_memory->readWord (m_registers->stackIndex ());
  m_registers->m_gen[SP] += 2;

  ___CBTPOP;
}

void processor_t::executePopRm (void) {
  ___CBTPUSH;

  const int modrmByte = nextByte ();

  executePop ();
  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    m_memory->writeWord (m_operandAddress->value (), m_operand1);
  } else {
    int regId = modrmByte & 7;
    m_registers->m_gen[regId] = m_operand1;
  }

  ___CBTPOP;
}

void processor_t::executePush (void) {
  ___CBTPUSH;

  /* TODO: What if stack pointer < 2 ???  */
  m_registers->m_gen[SP] -= 2;
  m_memory->writeWord (m_registers->stackIndex (), m_operand1);

  ___CBTPOP;
}

void processor_t::executePushPopReg (int firstByte) {
  ___CBTPUSH;

  int regIndex = firstByte & 7;
  if ((firstByte & 8) != 0) {
    executePop ();
    m_registers->m_gen[regIndex] = m_operand1;
  } else {
    m_operand1 = m_registers->m_gen[regIndex];
    executePush ();
  }

  ___CBTPOP;
}

void processor_t::executePushPopSeg (int firstByte) {
  ___CBTPUSH;

  int segId = firstByte >> 3 & 3;
  if ((firstByte & 1) != 0) {
    executePop ();
    m_registers->m_seg[segId] = m_operand1;
  } else {
    m_operand1 = m_registers->m_seg[segId];
    executePush ();
  }

  ___CBTPOP;
}

void processor_t::executePushRm (int modrmByte) {
  ___CBTPUSH;

  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    m_operand1 = m_memory->readWord (m_operandAddress->value ());
  } else {
    int regId = modrmByte & 7;
    m_operand1 = m_registers->m_gen[regId];
  }
  executePush ();

  ___CBTPOP;
}

void processor_t::executeReturn (int firstByte) {
  ___CBTPUSH;

  int imm = firstByte & 1 ? 0 : nextWord ();
  executePop ();
  m_registers->m_ip = m_operand1;
  m_registers->m_gen[SP] += imm;

  ___CBTPOP;
}

void processor_t::executeReturnFar (int firstByte) {
  ___CBTPUSH;

  int imm = firstByte & 1 ? 0 : nextWord ();
  executePop ();
  m_registers->m_ip = m_operand1;
  executePop ();
  m_registers->m_seg[CS] = m_operand1;
  m_registers->m_gen[SP] += imm;

  ___CBTPOP;
}

void processor_t::executeShiftInstruction (int opcode, bool wide) {
  ___CBTPUSH;

  if (m_operand2 > 0) {
    int r;
    switch (opcode) {
    case OPC_ROL:
      r = m_operand2 & operandWidth[wide] - 1;
      m_registers->m_flags &= ROTATE_FLAGS_CLEAR_MASK;
      m_operand1 <<= r;
      if ((m_operand1 & carryMask[wide]) != 0) m_registers->m_flags |= F_CARRY;
      if (!(m_operand1 & signMask[wide]) != !flag (F_CARRY)) m_registers->m_flags |= F_OVERFLOW;
      m_operand1 |= m_operand1 >> operandWidth[wide];
      m_operand1 &= valueMask[wide];
      break;

    case OPC_ROR:
      r = m_operand2 & operandWidth[wide] - 1;
      m_registers->m_flags &= ROTATE_FLAGS_CLEAR_MASK;
      m_operand1 |= m_operand1 << operandWidth[wide];
      m_operand1 >>= r;
      m_operand1 &= valueMask[wide];
      if ((m_operand1 & signMask[wide]) != 0) m_registers->m_flags |= F_CARRY;
      switch (m_operand1 >> operandWidth[wide] - 2) {
      case 1:
      case 2: m_registers->m_flags |= F_OVERFLOW;
      }
      break;

    case OPC_RCL:
      r = m_operand2 % (operandWidth[wide] + 1);
      if (r != 0) {
        if (flag (F_CARRY) != 0) m_operand1 |= carryMask[wide];
        m_registers->m_flags &= ROTATE_FLAGS_CLEAR_MASK;
        m_operand1 <<= r;
        m_operand1 |= m_operand1 >> operandWidth[wide] + 1;
        if ((m_operand1 & carryMask[wide]) != 0) m_registers->m_flags |= F_CARRY;
        if (!(m_operand1 & signMask[wide]) != !flag (F_CARRY)) m_registers->m_flags |= F_OVERFLOW;
        m_operand1 &= valueMask[wide];
      }
      break;

    case OPC_RCR:
      r = m_operand2 % (operandWidth[wide] + 1);
      if (r != 0) {
        if (flag (F_CARRY) != 0) m_operand1 |= carryMask[wide];
        m_registers->m_flags &= ROTATE_FLAGS_CLEAR_MASK;
        m_operand1 |= m_operand1 << operandWidth[wide] + 1;
        m_operand1 >>= r;
        if ((m_operand1 & carryMask[wide]) != 0) m_registers->m_flags |= F_CARRY;
        m_operand1 &= valueMask[wide];
        switch (m_operand1 >> operandWidth[wide] - 2) {
        case 1:
        case 2: m_registers->m_flags |= F_OVERFLOW;
        }
      }
      break;

    case OPC_SAL:
    case OPC_SHL: /* TODO: Nog te testen. */
      m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK;
      m_operand1 <<= m_operand2;
      if ((m_operand1 & carryMask[wide]) != 0) m_registers->m_flags |= F_CARRY;
      m_operand1 &= valueMask[wide];
      if ((m_operand1 & signMask[wide]) != 0) m_registers->m_flags |= F_SIGN;
      if (!flag (F_SIGN) != !flag (F_CARRY)) m_registers->m_flags |= F_OVERFLOW;
      if (parity (m_operand1)) m_registers->m_flags |= F_PARITY;
      if (m_operand1 == 0) m_registers->m_flags |= F_ZERO;
      break;

    case OPC_SHR: /* TODO: Nog te testen. */
      m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK;
      if (m_operand2 == 1 && (m_operand1 & signMask[wide]) != 0) m_registers->m_flags |= F_OVERFLOW;
      m_operand1 >>= m_operand2 - 1;
      if (m_operand1 & 1) m_registers->m_flags |= F_CARRY;
      m_operand1 >>= 1;
      if (parity (m_operand1)) m_registers->m_flags |= F_PARITY;
      if (m_operand1 == 0) m_registers->m_flags |= F_ZERO;
      break;

    case OPC_SAR: /* TODO: Nog te testen. */
      m_registers->m_flags &= STATUS_FLAGS_CLEAR_MASK;
      if ((m_operand1 & signMask[wide]) != 0) {
        m_operand1 |= signExtension[wide];
        m_registers->m_flags |= F_SIGN;
      }
      m_operand1 >>= m_operand2 - 1;
      if (m_operand1 & 1) m_registers->m_flags |= F_CARRY;
      m_operand1 >>= 1;
      if (parity (m_operand1)) m_registers->m_flags |= F_PARITY;
      if (m_operand1 == 0) m_registers->m_flags |= F_ZERO;
      break;
    }
  }

  ___CBTPOP;
}

void processor_t::executeShiftInstructionRm (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  const int modrmByte = nextByte ();
  const int opcode = modrmByte >> 3 & 7;

  m_operand2 = (firstByte & 2) != 0 ? readRegister (CX, false) : 1;
  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    int operandAddress = m_operandAddress->value ();
    m_operand1 = m_memory->read (operandAddress, wide);
    executeShiftInstruction (opcode, wide);
    m_memory->write (operandAddress, m_operand1, wide);
  } else {
    int regId = modrmByte & 7;
    m_operand1 = readRegister (regId, wide);
    executeShiftInstruction (opcode, wide);
    writeRegister (regId, m_operand1, wide);
  }

  ___CBTPOP;
}

void processor_t::executeStringCompare (int step, bool wide) {
  ___CBTPUSH;

  if (m_repeat != -1) {
    if (m_registers->m_gen[CX] != 0) {

      const bool repz = m_repeat == OPC_REPZ;
      if (flag (F_TRAP) == 0) {

        do {
          m_operand1 = m_memory->read (m_registers->sourceIndex (), wide);
          m_operand2 = m_memory->read (m_registers->destinationIndex (), wide);
          m_registers->m_gen[SI] += step;
          m_registers->m_gen[DI] += step;
          --m_registers->m_gen[CX];
        } while (m_registers->m_gen[CX] != 0 && m_operand1 == m_operand2 == repz);

      } else {

        m_operand1 = m_memory->read (m_registers->sourceIndex (), wide);
        m_operand2 = m_memory->read (m_registers->destinationIndex (), wide);
        m_registers->m_gen[SI] += step;
        m_registers->m_gen[DI] += step;
        --m_registers->m_gen[CX];
        if (m_registers->m_gen[CX] != 0 && m_operand1 == m_operand2 == repz) {
          m_registers->m_ip -= 2;
        }

      }
      executeAGI (OPC_CMP, wide);

    }
  } else {
    m_operand1 = m_memory->read (m_registers->sourceIndex (), wide);
    m_operand2 = m_memory->read (m_registers->destinationIndex (), wide);
    m_registers->m_gen[SI] += step;
    m_registers->m_gen[DI] += step;
    executeAGI (OPC_CMP, wide);
  }

  ___CBTPOP;
}

void processor_t::executeStringInstruction (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  int step = flag (F_DIRECTION) != 0 ? -1 : 1;
  step <<= wide;

  switch (firstByte & 0xFE) {
  case OPC_MOVSB: executeStringMove (step, wide); break;
  case OPC_CMPSB: executeStringCompare (step, wide); break;
  case OPC_STOSB: executeStringStore (step, wide); break;
  case OPC_LODSB: executeStringLoad (step, wide); break;
  case OPC_SCASB: executeStringScan (step, wide);
  }

  ___CBTPOP;
}

void processor_t::executeStringLoad (int step, bool wide) {
  ___CBTPUSH;

  if (m_repeat != -1) {
    if (m_registers->m_gen[CX] != 0) {

      if (flag (F_TRAP) == 0) {
        m_registers->m_gen[SI] += step * (m_registers->m_gen[CX] - 1);
        m_operand1 = m_memory->read (m_registers->sourceIndex (), wide);
        writeRegister (0, m_operand1, wide);
        m_registers->m_gen[CX] = 0;
      } else {
        m_operand1 = m_memory->read (m_registers->sourceIndex (), wide);
        writeRegister (0, m_operand1, wide);
        --m_registers->m_gen[CX];
        if (m_registers->m_gen[CX] != 0) {
          m_registers->m_ip -= 2;
        }
      }
      m_registers->m_gen[SI] += step;

    }
  } else {
    m_operand1 = m_memory->read (m_registers->sourceIndex (), wide);
    writeRegister (0, m_operand1, wide);
    m_registers->m_gen[SI] += step;
  }

  ___CBTPOP;
}

void processor_t::executeStringMove (int step, bool wide) {
  ___CBTPUSH;

  if (m_repeat != -1) {
    if (m_registers->m_gen[CX] != 0) {

      if (flag (F_TRAP) == 0) {

        do {
          m_operand1 = m_memory->read (m_registers->sourceIndex (), wide);
          m_memory->write (m_registers->destinationIndex (), m_operand1, wide);
          m_registers->m_gen[SI] += step;
          m_registers->m_gen[DI] += step;
          --m_registers->m_gen[CX];
        } while (m_registers->m_gen[CX] != 0);

      } else {

        m_operand1 = m_memory->read (m_registers->sourceIndex (), wide);
        m_memory->write (m_registers->destinationIndex (), m_operand1, wide);
        m_registers->m_gen[SI] += step;
        m_registers->m_gen[DI] += step;
        --m_registers->m_gen[CX];
        if (m_registers->m_gen[CX] != 0) {
          m_registers->m_ip -= 2;
        }

      }

    }
  } else {
    m_operand1 = m_memory->read (m_registers->sourceIndex (), wide);
    m_memory->write (m_registers->destinationIndex (), m_operand1, wide);
    m_registers->m_gen[SI] += step;
    m_registers->m_gen[DI] += step;
  }

  ___CBTPOP;
}

void processor_t::executeStringScan (int step, bool wide) {
  ___CBTPUSH;

  if (m_repeat != -1) {
    if (m_registers->m_gen[CX] != 0) {

      m_operand1 = readRegister (0, wide);
      const bool repz = m_repeat == OPC_REPZ;
      if (flag (F_TRAP) == 0) {

        do {
          m_operand2 = m_memory->read (m_registers->destinationIndex (), wide);
          m_registers->m_gen[DI] += step;
          --m_registers->m_gen[CX];
        } while (m_registers->m_gen[CX] != 0 && m_operand1 == m_operand2 == repz);

      } else {

        m_operand2 = m_memory->read (m_registers->destinationIndex (), wide);
        m_registers->m_gen[DI] += step;
        --m_registers->m_gen[CX];
        if (m_registers->m_gen[CX] != 0 && m_operand1 == m_operand2 == repz) {
          m_registers->m_ip -= 2;
        }

      }
      executeAGI (OPC_CMP, wide);

    }
  } else {
    m_operand1 = readRegister (0, wide);
    m_operand2 = m_memory->read (m_registers->destinationIndex (), wide);
    m_registers->m_gen[DI] += step;
    executeAGI (OPC_CMP, wide);
  }

  ___CBTPOP;
}

void processor_t::executeStringStore (int step, bool wide) {
  ___CBTPUSH;

  if (m_repeat != -1) {
    if (m_registers->m_gen[CX] != 0) {

      m_operand1 = readRegister (0, wide);
      if (flag (F_TRAP) == 0) {

        do {
          m_memory->write (m_registers->destinationIndex (), m_operand1, wide);
          m_registers->m_gen[DI] += step;
          --m_registers->m_gen[CX];
        } while (m_registers->m_gen[CX] != 0);

      } else {

        m_memory->write (m_registers->destinationIndex (), m_operand1, wide);
        m_registers->m_gen[DI] += step;
        --m_registers->m_gen[CX];
        if (m_registers->m_gen[CX] != 0) {
          m_registers->m_ip -= 2;
        }

      }

    }
  } else {
    m_operand1 = readRegister (0, wide);
    m_memory->write (m_registers->destinationIndex (), m_operand1, wide);
    m_registers->m_gen[DI] += step;
  }

  ___CBTPOP;
}

void processor_t::executeSub (bool wide, bool carry) {
  ___CBTPUSH;

  int64_t x, y;
  int64_t u = m_operand1 & 0xF;
  int64_t v = m_operand2 & 0xF;

  if (wide) {
    x = (int16_t) m_operand1;
    y = (int16_t) m_operand2;
  } else {
    x = (int8_t) m_operand1;
    y = (int8_t) m_operand2;
  }

  m_operand1 -= m_operand2 + carry;
  x -= y + carry;

  if (u - v - carry < 0) m_registers->m_flags |= F_ADJUST;
  if (m_operand1 < 0) m_registers->m_flags |= F_CARRY;
  if (x < signedMin[wide] || x > signedMax[wide]) m_registers->m_flags |= F_OVERFLOW;
  m_operand1 &= valueMask[wide];

  ___CBTPOP;
}

void processor_t::executeTestAccImm (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;

  m_operand1 = readRegister (0, wide);
  m_operand2 = wide ? nextWord () : nextByte ();
  executeAGI (OPC_AND, wide);

  ___CBTPOP;
}

void processor_t::executeTestRegRm (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  const int modrmByte = nextByte ();

  m_operand1 = readRegister (modrmByte >> 3 & 7, wide);
  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    m_operand2 = m_memory->read (m_operandAddress->value (), wide);
  } else {
    m_operand2 = readRegister (modrmByte & 7, wide);
  }
  executeAGI (OPC_AND, wide);

  ___CBTPOP;
}

void processor_t::executeXchgAxReg (int firstByte) {
  ___CBTPUSH;

  const int regId = firstByte & 7;
  m_operand1 = m_registers->m_gen[AX];
  m_registers->m_gen[AX] = m_registers->m_gen[regId];
  m_registers->m_gen[regId] = m_operand1;

  ___CBTPOP;
}

void processor_t::executeXchgRegRm (int firstByte) {
  ___CBTPUSH;

  const bool wide = firstByte & 1;
  const int modrmByte = nextByte ();
  const int rrrId = modrmByte >> 3 & 7;

  m_operand1 = readRegister (rrrId, wide);
  if (instructionHasMemoryOperand (modrmByte)) {
    calculateOperandAddress (modrmByte);
    m_operand2 = m_memory->read (m_operandAddress->value (), wide);
    m_memory->write (m_operandAddress->value (), m_operand1, wide);
  } else {
    m_operand2 = readRegister (modrmByte & 7, wide);
    writeRegister (modrmByte & 7, m_operand1, wide);
  }
  writeRegister (rrrId, m_operand2, wide);

  ___CBTPOP;
}

int processor_t::nextByte (void) {
  ___CBTPUSH;

  int result = m_memory->readByte (m_registers->instructionIndex ());
  ++m_registers->m_ip;

  ___CBTPOP;
  return result;
}

int processor_t::nextByteAsWord (void) {
  ___CBTPUSH;

  int result = m_memory->readByteAsWord (m_registers->instructionIndex ());
  ++m_registers->m_ip;

  ___CBTPOP;
  return result;
}

int processor_t::nextWord (void) {
  ___CBTPUSH;

  int result = m_memory->readWord (m_registers->instructionIndex ());
  m_registers->m_ip += 2;

  ___CBTPOP;
  return result;
}

bool processor_t::parity (int64_t value) {
  ___CBTPUSH;

  int count = 0;
  int64_t mask = 0x80;
  for (int i = 0; i < 8; ++i) {
    if ((value & mask) != 0) {
      ++count;
    }
    mask >>= 1;
  }

  ___CBTPOP;
  return !(count & 1);
}

int processor_t::readRegister (int regId, bool wide) {
  ___CBTPUSH;

  int result;
  if (wide) {
    result = m_registers->m_gen[regId];
  } else {
    result = m_registers->m_gen[regId & 3];
    if ((regId & 4) != 0) {
      result >>= 8;
    } else {
      result &= 0xFF;
    }
  }

  ___CBTPOP;
  return result;
}

registers_t* processor_t::registers (void) {
  return m_registers;
}

void processor_t::setGeneralPurposeRegister (int index, unsigned short val) {
  ___CBTPUSH;

  m_registers->m_gen[index] = val;

  ___CBTPOP;
}

void processor_t::setInstructionPointer (unsigned short val) {
  ___CBTPUSH;

  m_registers->m_ip = val;

  ___CBTPOP;
}

void processor_t::setSegmentRegister (int index, unsigned short val) {
  ___CBTPUSH;

  m_registers->m_seg[index] = val;

  ___CBTPOP;
}

void processor_t::setTrap (void) {
  ___CBTPUSH;

  m_registers->m_flags |= F_TRAP;

  ___CBTPOP;
}

void processor_t::writeRegister (int regId, int value, bool wide) {
  ___CBTPUSH;

  if (wide) {
    m_registers->m_gen[regId] = value;
  } else {
    int index = regId & 3;
    if ((regId & 4) != 0) {
      m_registers->m_gen[index] &= 0x00FF;
      value <<= 8;
    } else {
      value &= 0xFF;
      m_registers->m_gen[index] &= 0xFF00;
    }
    m_registers->m_gen[index] |= value;
  }

  ___CBTPOP;
}
