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

#include "compiler.h"
#include "instructionCodes.h"
#include <lombok/base/errors.h>

REFL_IMPL (compiler_t, object_t)

compiler_t::compiler_t (memory_t* memory) : m_memory (memory) {
  ___CBTPUSH;

  memory->reserve ();

  ___CBTPOP;
}

compiler_t::~compiler_t (void) {
  ___CBTPUSH;

  m_memory->dump ();

  ___CBTPOP;
}

void compiler_t::compile (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  switch (instruction->group ()) {
  case GRP_AAM: compileAam (address, instruction); break;
  case GRP_ADD: compileAdd (address, instruction); break;
  case GRP_CALL: compileCall (address, instruction); break;
  case GRP_CONJMP: compileConditionalJump (address, instruction); break;
  case GRP_IN: compileIn (address, instruction); break;
  case GRP_INC: compileInc (address, instruction); break;
  case GRP_INT: compileInt (address, instruction); break;
  case GRP_JMP: compileJump (address, instruction); break;
  case GRP_LEA: compileLea (address, instruction); break;
  case GRP_MISC: compileMisc (address, instruction); break;
  case GRP_MOV: compileMov (address, instruction); break;
  case GRP_NOT: compileNot (address, instruction); break;
  case GRP_OUT: compileOut (address, instruction); break;
  case GRP_PUSH: compilePush (address, instruction); break;
  case GRP_RET: compileRet (address, instruction); break;
  case GRP_SHIFT: compileShift (address, instruction); break;
  case GRP_TEST: compileTest (address, instruction); break;
  case GRP_XCHG: compileXchg (address, instruction);
  }

  ___CBTPOP;
}

/* AAM, AAD */
void compiler_t::compileAam (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  const int index = address->value ();

  if (operand != NULL) {
    m_memory->writeByte (index + 1, operand->value ());
  } else {
    m_memory->writeByte (index + 1, 10);
  }
  m_memory->writeByte (index, instruction->operationCode ());
  address->add (2);

  ___CBTPOP;
}

/* ADD (0), OR (1), ADC (2), SBB (3), AND (4), SUB (5), XOR (6), CMP (7) */
void compiler_t::compileAdd (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  int b, dispLen;
  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const bool wide = operand1->isWide ();
  const int index = address->value ();

  if (!operand2->isImmediate () || operand1->isAccumulator ()) {
    b = instruction->operationCode () << 3 | wide;
    if (operand2->isImmediate ()) {
      m_memory->writeByte (index, b | 4);
      m_memory->write (index + 1, operand2->value (), wide);
      address->add (2 + wide);
    } else if (operand2->isPointer ()) {
      m_memory->writeByte (index, b | 2);
      dispLen = writeModrmAndDisplacement (index + 1, operand1->value (), operand2);
      address->add (2 + dispLen);
    } else {
      m_memory->writeByte (index, b);
      dispLen = writeModrmAndDisplacement (index + 1, operand2->value (), operand1);
      address->add (2 + dispLen);
    }
  } else {
    const bool wideData = wide && operand2->value () > 0x7F && operand2->value () < 0xFF80;
    b = 0x80 | wide | !wideData << 1;
    m_memory->writeByte (index, b);
    dispLen = writeModrmAndDisplacement (index + 1, instruction->operationCode (), operand1);
    m_memory->write (index + dispLen + 2, operand2->value (), wideData);
    address->add (3 + dispLen + wideData);
  }

  ___CBTPOP;
}

/* CALL (2), CALL FAR (3) */
void compiler_t::compileCall (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const int index = address->value ();

  if (operand2 != NULL) {
    m_memory->writeByte (index, 0x9A);
    m_memory->writeWord (index + 3, operand1->value ());
    m_memory->writeWord (index + 1, operand2->value ());
    address->add (5);
  } else if (operand1->isImmediate ()) {
    m_memory->writeByte (index, 0xE8);
    int value = operand1->value () - address->offset () - 3;
    m_memory->writeWord (index + 1, value);
    address->add (3);
  } else {
    m_memory->writeByte (index, 0xFF);
    int dispLen = writeModrmAndDisplacement (index + 1, instruction->operationCode (), operand1);
    address->add (2 + dispLen);
  }

  ___CBTPOP;
}

/* JO, JNO, JB, JNB, JZ, JNZ, JBE, JA, JS, JNS,
   JPE, JPO, JL, JGE, JLE, JG, LOOPNZ, LOOPZ, LOOP, JCXZ */
void compiler_t::compileConditionalJump (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  /* operand->value() should be precalculated. */
  operand_t* const operand = instruction->operand1 ();
  const int index = address->value ();

  m_memory->writeByte (index, instruction->operationCode ());
  m_memory->writeByte (index + 1, operand->value ());
  address->add (2);

  ___CBTPOP;
}

/* IN (0) */
void compiler_t::compileIn (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand2 = instruction->operand2 ();
  const bool wide = instruction->operand1 ()->isWide ();
  const int index = address->value ();

  if (operand2->isImmediate ()) {
    m_memory->writeByte (index, 0xE4 | wide);
    m_memory->writeByte (index + 1, operand2->value ());
    address->add (2);
  } else {
    m_memory->writeByte (index, 0xEC | wide);
    address->add (1);
  }

  ___CBTPOP;
}

/* INC (0), DEC (1) */
void compiler_t::compileInc (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  int b, dispLen;
  operand_t* const operand = instruction->operand1 ();
  const bool wide = operand->isWide ();
  const int index = address->value ();

  if (operand->isRegister () && wide) {
    b = 0x40 | instruction->operationCode () << 3 | operand->value ();
    dispLen = -1;
  } else {
    b = 0xFE | wide;
    dispLen = writeModrmAndDisplacement (index + 1, instruction->operationCode (), operand);
  }
  m_memory->writeByte (index, b);
  address->add (2 + dispLen);

  ___CBTPOP;
}

/* INT (0) */
void compiler_t::compileInt (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  const int index = address->value ();

  if (operand->value () == 3) {
    m_memory->writeByte (index, 0xCC);
    address->add (1);
  } else {
    m_memory->writeByte (index, 0xCD);
    m_memory->writeByte (index + 1, operand->value ());
    address->add (2);
  }

  ___CBTPOP;
}

/* JUMP (4), JUMP FAR (5) */
void compiler_t::compileJump (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const int index = address->value ();

  if (operand2 != NULL) {
    m_memory->writeByte (index, 0xEA);
    m_memory->writeWord (index + 3, operand1->value ());
    m_memory->writeWord (index + 1, operand2->value ());
    address->add (5);
  } else if (operand1->isImmediate ()) {
    int value = operand1->value () - address->offset () - 2;
    if (value < -0x80 || value > 0x7F) {
      m_memory->writeByte (index, 0xE9);
      m_memory->writeWord (index + 1, value - 1);
      address->add (3);
    } else {
      m_memory->writeByte (index, 0xEB);
      m_memory->writeByte (index + 1, value);
      address->add (2);
    }
  } else {
    m_memory->writeByte (index, 0xFF);
    int dispLen = writeModrmAndDisplacement (index + 1, instruction->operationCode (), operand1);
    address->add (2 + dispLen);
  }

  ___CBTPOP;
}

/* LEA, LES, LDS */
void compiler_t::compileLea (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  int dispLen;
  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const int index = address->value ();

  m_memory->writeByte (index, instruction->operationCode ());
  dispLen = writeModrmAndDisplacement (index + 1, operand1->value (), operand2);
  address->add (2 + dispLen);

  ___CBTPOP;
}

/* ES:, DAA, CS:, DAS, SS:, AAA, DS:, AAS, NOP, CBW, CWD, WAIT, PUSHF, POPF, SAHF, LAHF,
   MOVSB, MOVSW, CMPSB, CMPSW, STOSB, STOSW, LODSB, LODSW, SCASB, SCASW, INTO, IRET, XLAT,
   LOCK, REPNZ, REPZ, HLT, CMC, CLC, STC, CLI, STI, CLD, STD */
void compiler_t::compileMisc (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  const int index = address->value ();

  m_memory->writeByte (index, instruction->operationCode ());
  address->add (1);

  ___CBTPOP;
}

/* MOV (0) */
void compiler_t::compileMov (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  int b, dispLen;
  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const bool wide = operand1->isWide ();
  const int index = address->value ();

  if (operand2->isImmediate ()) {
    if (operand1->isRegister ()) {
      b = (0x16 | wide) << 3 | operand1->value ();
      dispLen = -1;
    } else {
      b = 0xC6 | wide;
      dispLen = writeModrmAndDisplacement (index + 1, instruction->operationCode (), operand1);
    }
    m_memory->writeByte (index, b);
    m_memory->write (index + dispLen + 2, operand2->value (), wide);
    address->add (3 + dispLen + wide);
  } else if (operand2->isSegRegister ()) {
    m_memory->writeByte (index, 0x8C);
    dispLen = writeModrmAndDisplacement (index + 1, operand2->value (), operand1);
    address->add (2 + dispLen);
  } else if (operand1->isSegRegister ()) {
    m_memory->writeByte (index, 0x8E);
    dispLen = writeModrmAndDisplacement (index + 1, operand1->value (), operand2);
    address->add (2 + dispLen);
  } else if (operand2->isPointer ()) {
    if (operand1->isAccumulator () && operand2->value () == 6) {
      m_memory->writeByte (index, 0xA0 | wide);
      m_memory->writeWord (index + 1, operand2->displacement ());
      address->add (3);
    } else {
      m_memory->writeByte (index, 0x8A | wide);
      dispLen = writeModrmAndDisplacement (index + 1, operand1->value (), operand2);
      address->add (2 + dispLen);
    }
  } else if (operand2->isAccumulator () && operand1->isPointer () && operand1->value () == 6) {
    m_memory->writeByte (index, 0xA2 | wide);
    m_memory->writeWord (index + 1, operand1->displacement ());
    address->add (3);
  } else {
    m_memory->writeByte (index, 0x88 | wide);
    dispLen = writeModrmAndDisplacement (index + 1, operand2->value (), operand1);
    address->add (2 + dispLen);
  }

  ___CBTPOP;
}

/* NOT (2), NEG (3), MUL (4), IMUL (5), DIV (6), IDIV (7) */
void compiler_t::compileNot (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  const bool wide = operand->isWide ();
  const int index = address->value ();

  m_memory->writeByte (index, 0xF6 | wide);
  int dispLen = writeModrmAndDisplacement (index + 1, instruction->operationCode (), operand);
  address->add (2 + dispLen);

  ___CBTPOP;
}

/* OUT (0) */
void compiler_t::compileOut (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  const bool wide = instruction->operand2 ()->isWide ();
  const int index = address->value ();

  if (operand1->isImmediate ()) {
    m_memory->writeByte (index, 0xE6 | wide);
    m_memory->writeByte (index + 1, operand1->value ());
    address->add (2);
  } else {
    m_memory->writeByte (index, 0xEE | wide);
    address->add (1);
  }

  ___CBTPOP;
}

/* PUSH (0), POP (1) */
void compiler_t::compilePush (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  int b, dispLen;
  operand_t* const operand = instruction->operand1 ();
  const int index = address->value ();

  if (operand->isSegRegister ()) {
    b = operand->value () << 3 | 6 | instruction->operationCode ();
    dispLen = -1;
  } else if (operand->isRegister ()) {
    b = (10 | instruction->operationCode ()) << 3 | operand->value ();
    dispLen = -1;
  } else if (instruction->operationCode () == 0) {
    b = 0xFF;
    dispLen = writeModrmAndDisplacement (index + 1, 6, operand);
  } else {
    b = 0x8F;
    dispLen = writeModrmAndDisplacement (index + 1, 0, operand);
  }
  m_memory->writeByte (index, b);
  address->add (2 + dispLen);

  ___CBTPOP;
}

/* RET (194), RETF (202) */
void compiler_t::compileRet (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  const int index = address->value ();

  if (operand != NULL) {
    m_memory->writeByte (index, instruction->operationCode ());
    m_memory->writeWord (index + 1, operand->value ());
    address->add (3);
  } else {
    m_memory->writeByte (index, instruction->operationCode () | 1);
    address->add (1);
  }

  ___CBTPOP;
}

/* ROL (0), ROR (1), RCL (2), RCR (3), SHL (4), SHR (5), SAL (6), SAR (7)
   Second operand is either 1 or CL. */
void compiler_t::compileShift (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  const int operand2Bit = instruction->operand2 ()->isRegister () << 1;
  const bool wide = operand1->isWide ();
  const int index = address->value ();

  m_memory->writeByte (index, 0xD0 | operand2Bit | wide);
  int dispLen = writeModrmAndDisplacement (index + 1, instruction->operationCode (), operand1);
  address->add (2 + dispLen);

  ___CBTPOP;
}

/* TEST (0) */
void compiler_t::compileTest (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  int dispLen;
  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const bool wide = operand1->isWide ();
  const int index = address->value ();

  if (operand2->isImmediate ()) {
    if (operand1->isAccumulator ()) {
      m_memory->writeByte (index, 0xA8 | wide);
      dispLen = -1;
    } else {
      m_memory->writeByte (index, 0xF6 | wide);
      dispLen = writeModrmAndDisplacement (index + 1, instruction->operationCode (), operand1);
    }
    m_memory->write (index + dispLen + 2, operand2->value (), wide);
    address->add (3 + dispLen + wide);
  } else {
    m_memory->writeByte (index, 0x84 | wide);
    dispLen = writeModrmAndDisplacement (index + 1, operand2->value (), operand1);
    address->add (2 + dispLen);
  }

  ___CBTPOP;
}

/* XCHG (0) */
void compiler_t::compileXchg (address_t* address, instruction_t* instruction) {
  ___CBTPUSH;

  int dispLen;
  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const bool wide = operand1->isWide ();
  const int index = address->value ();

  if (wide && operand1->isAccumulator () && operand2->isRegister ()) {
    m_memory->writeByte (index, 0x90 | operand2->value ());
    address->add (1);
  } else {
    m_memory->writeByte (index, 0x86 | wide);
    dispLen = writeModrmAndDisplacement (index + 1, operand1->value (), operand2);
    address->add (2 + dispLen);
  }

  ___CBTPOP;
}

int compiler_t::writeDisplacement (int index, operand_t* pointer) {
  ___CBTPUSH;

  int numberOfBytesWritten = pointer->value () >> 6;
  switch (numberOfBytesWritten) {
  case 0:
    if ((pointer->value () & 7) == 6) {
      m_memory->writeWord (index, pointer->displacement ());
      numberOfBytesWritten = 2;
    }
    break;
  case 1: m_memory->writeByte (index, pointer->displacement ()); break;
  case 2: m_memory->writeWord (index, pointer->displacement ());
  }

  ___CBTPOP;
  return numberOfBytesWritten;
}

int compiler_t::writeModrmAndDisplacement (int modrmIndex, int regOrOpCode, operand_t* regOrMemOperand) {
  ___CBTPUSH;

  int displacementSize;
  int modrm = regOrOpCode << 3 | regOrMemOperand->value ();
  if (regOrMemOperand->isPointer ()) {
    displacementSize = writeDisplacement (modrmIndex + 1, regOrMemOperand);
  } else {
    displacementSize = 0;
    modrm |= 0xC0;
  }
  m_memory->writeByte (modrmIndex, modrm);

  ___CBTPOP;
  return displacementSize;
}
