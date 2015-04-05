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
#include "compiler.h"
#include "instructionCodes.h"

REFL_IMPL (compiler_t, object_t)

compiler_t::compiler_t (void) {
  ___CBTPUSH;
  ___CBTPOP;
}

compiler_t::~compiler_t (void) {
  ___CBTPUSH;
  ___CBTPOP;
}

void compiler_t::compile (asmWriter_t* outstream, programme_t* programme, int interruptNumber) {
  ___CBTPUSH;

  writeHeader (outstream, programme, interruptNumber);

  arrayList_t* byteBlocks = programme->byteBlocks ();
  int blockCount = byteBlocks->length ();
  for (int i = 0; i < blockCount; ++i) {
    byteBlock_t* block = (byteBlock_t*) byteBlocks->get (i);
    switch (block->type ()) {
    case byteBlock_t::TYPE_DATA:
      compileData (outstream, (data_t*) block);
      break;

    case byteBlock_t::TYPE_INSTRUCTION:
      switch (((instruction_t*) block)->group ()) {
      case GRP_AAM: compileAam (outstream, (instruction_t*) block); break;
      case GRP_ADD: compileAdd (outstream, (instruction_t*) block); break;
      case GRP_CALL: compileCall (outstream, (instruction_t*) block); break;
      case GRP_CONJMP: compileConditionalJump (outstream, (instruction_t*) block); break;
      case GRP_IN: compileIn (outstream, (instruction_t*) block); break;
      case GRP_INC: compileInc (outstream, (instruction_t*) block); break;
      case GRP_INT: compileInt (outstream, (instruction_t*) block); break;
      case GRP_JMP: compileJump (outstream, (instruction_t*) block); break;
      case GRP_LEA: compileLea (outstream, (instruction_t*) block); break;
      case GRP_MISC: compileMisc (outstream, (instruction_t*) block); break;
      case GRP_MOV: compileMov (outstream, (instruction_t*) block); break;
      case GRP_NOT: compileNot (outstream, (instruction_t*) block); break;
      case GRP_OUT: compileOut (outstream, (instruction_t*) block); break;
      case GRP_PUSH: compilePush (outstream, (instruction_t*) block); break;
      case GRP_RET: compileRet (outstream, (instruction_t*) block); break;
      case GRP_SHIFT: compileShift (outstream, (instruction_t*) block); break;
      case GRP_TEST: compileTest (outstream, (instruction_t*) block); break;
      case GRP_XCHG: compileXchg (outstream, (instruction_t*) block);
      }
      break;
    }
  }

  ___CBTPOP;
}

void compiler_t::compileAam (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  outstream->writeByte (instruction->operationCode ());
  if (operand != NULL) {
    outstream->writeByte (operand->value ());
  } else {
    outstream->writeByte (10);
  }

  ___CBTPOP;
}

void compiler_t::compileAdd (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  int firstByte;
  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const bool wide = operand1->isWide ();

  if (!operand2->isImmediate () || operand1->isAccumulator ()) {
    firstByte = instruction->operationCode () << 3 | wide;
    if (operand2->isImmediate ()) {
      outstream->writeByte (firstByte | 4);
      outstream->write (operand2->value (), wide);
    } else if (operand2->isPointer ()) {
      outstream->writeByte (firstByte | 2);
      writeModrm (outstream, operand1->value (), operand2);
    } else {
      outstream->writeByte (firstByte);
      writeModrm (outstream, operand2->value (), operand1);
    }
  } else {
    const int immValue = operand2->value () & 0xFFFF;
    const bool wideData = wide && immValue > 0x7F && immValue < 0xFF80;
    firstByte = 0x80 | wide | !wideData << 1;
    outstream->writeByte (firstByte);
    writeModrm (outstream, instruction->operationCode (), operand1);
    outstream->write (immValue, wideData);
  }

  ___CBTPOP;
}

void compiler_t::compileCall (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();

  if (operand2 != NULL) {
    outstream->writeByte (0x9A);
    outstream->writeWord (operand2->value ());
    outstream->writeWord (operand1->value ());
  } else if (operand1->isImmediate ()) {
    outstream->writeByte (0xE8);
    outstream->writeWord (operand1->value () - instruction->location () - 3);
  } else {
    outstream->writeByte (0xFF);
    writeModrm (outstream, instruction->operationCode (), operand1);
  }

  ___CBTPOP;
}

void compiler_t::compileConditionalJump (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  outstream->writeByte (instruction->operationCode ());
  outstream->writeByte (operand->value () - instruction->location () - 2);

  ___CBTPOP;
}

void compiler_t::compileData (asmWriter_t* outstream, data_t* data) {
  ___CBTPUSH;

  const int size = data->size ();
  for (int i = 0; i < size; ++i) {
    outstream->writeByte (data->getByte (i));
  }

  ___CBTPOP;
}

void compiler_t::compileIn (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand2 = instruction->operand2 ();
  const bool wide = instruction->operand1 ()->isWide ();
  if (operand2->isImmediate ()) {
    outstream->writeByte (0xE4 | wide);
    outstream->writeByte (operand2->value ());
  } else {
    outstream->writeByte (0xEC | wide);
  }

  ___CBTPOP;
}

void compiler_t::compileInc (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  const bool wide = operand->isWide ();

  if (operand->isRegister () && wide) {
    outstream->writeByte (0x40 | instruction->operationCode () << 3 | operand->value ());
  } else {
    outstream->writeByte (0xFE | wide);
    writeModrm (outstream, instruction->operationCode (), operand);
  }

  ___CBTPOP;
}

void compiler_t::compileInt (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  if (operand->value () == 3) {
    outstream->writeByte (0xCC);
  } else {
    outstream->writeByte (0xCD);
    outstream->writeByte (operand->value ());
  }

  ___CBTPOP;
}

void compiler_t::compileJump (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();

  if (operand2 != NULL) {
    outstream->writeByte (0xEA);
    outstream->writeWord (operand2->value ());
    outstream->writeWord (operand1->value ());
  } else if (operand1->isImmediate ()) {
    int value = operand1->value () - instruction->location () - instruction->size ();
    if (instruction->size () == 3) {
      outstream->writeByte (0xE9);
      outstream->writeWord (value);
    } else {
      outstream->writeByte (0xEB);
      outstream->writeByte (value);
    }
  } else {
    outstream->writeByte (0xFF);
    writeModrm (outstream, instruction->operationCode (), operand1);
  }

  ___CBTPOP;
}

void compiler_t::compileLea (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  outstream->writeByte (instruction->operationCode ());
  writeModrm (outstream, operand1->value (), operand2);

  ___CBTPOP;
}

void compiler_t::compileMisc (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  outstream->writeByte (instruction->operationCode ());

  ___CBTPOP;
}

void compiler_t::compileMov (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const bool wide = operand1->isWide ();

  if (operand2->isImmediate ()) {
    if (operand1->isRegister ()) {
      outstream->writeByte ((0x16 | wide) << 3 | operand1->value ());
    } else {
      outstream->writeByte (0xC6 | wide);
      writeModrm (outstream, instruction->operationCode (), operand1);
    }
    outstream->write (operand2->value (), wide);
  } else if (operand2->isSegRegister ()) {
    outstream->writeByte (0x8C);
    writeModrm (outstream, operand2->value (), operand1);
  } else if (operand1->isSegRegister ()) {
    outstream->writeByte (0x8E);
    writeModrm (outstream, operand1->value (), operand2);
  } else if (operand2->isPointer ()) {
    if (operand1->isAccumulator () && operand2->value () == 6) {
      outstream->writeByte (0xA0 | wide);
      outstream->writeWord (operand2->displacement ());
    } else {
      outstream->writeByte (0x8A | wide);
      writeModrm (outstream, operand1->value (), operand2);
    }
  } else if (operand2->isAccumulator () && operand1->isPointer () && operand1->value () == 6) {
    outstream->writeByte (0xA2 | wide);
    outstream->writeWord (operand1->displacement ());
  } else {
    outstream->writeByte (0x88 | wide);
    writeModrm (outstream, operand2->value (), operand1);
  }

  ___CBTPOP;
}

void compiler_t::compileNot (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  outstream->writeByte (0xF6 | operand->isWide ());
  writeModrm (outstream, instruction->operationCode (), operand);

  ___CBTPOP;
}

void compiler_t::compileOut (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  const bool wide = instruction->operand2 ()->isWide ();
  if (operand1->isImmediate ()) {
    outstream->writeByte (0xE6 | wide);
    outstream->writeByte (operand1->value ());
  } else {
    outstream->writeByte (0xEE | wide);
  }

  ___CBTPOP;
}

void compiler_t::compilePush (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  if (operand->isSegRegister ()) {
    outstream->writeByte (operand->value () << 3 | 6 | instruction->operationCode ());
  } else if (operand->isRegister ()) {
    outstream->writeByte ((10 | instruction->operationCode ()) << 3 | operand->value ());
  } else if (instruction->operationCode () == 0) {
    outstream->writeByte (0xFF);
    writeModrm (outstream, 6, operand);
  } else {
    outstream->writeByte (0x8F);
    writeModrm (outstream, 0, operand);
  }

  ___CBTPOP;
}

void compiler_t::compileRet (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand = instruction->operand1 ();
  if (operand != NULL) {
    outstream->writeByte (instruction->operationCode ());
    outstream->writeWord (operand->value ());
  } else {
    outstream->writeByte (instruction->operationCode () | 1);
  }

  ___CBTPOP;
}

void compiler_t::compileShift (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  const int operand2Bit = instruction->operand2 ()->isRegister () << 1;
  const bool wide = operand1->isWide ();
  outstream->writeByte (0xD0 | operand2Bit | wide);
  writeModrm (outstream, instruction->operationCode (), operand1);

  ___CBTPOP;
}

void compiler_t::compileTest (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const bool wide = operand1->isWide ();

  if (operand2->isImmediate ()) {
    if (operand1->isAccumulator ()) {
      outstream->writeByte (0xA8 | wide);
    } else {
      outstream->writeByte (0xF6 | wide);
      writeModrm (outstream, instruction->operationCode (), operand1);
    }
    outstream->write (operand2->value (), wide);
  } else {
    outstream->writeByte (0x84 | wide);
    writeModrm (outstream, operand2->value (), operand1);
  }

  ___CBTPOP;
}

void compiler_t::compileXchg (asmWriter_t* outstream, instruction_t* instruction) {
  ___CBTPUSH;

  operand_t* const operand1 = instruction->operand1 ();
  operand_t* const operand2 = instruction->operand2 ();
  const bool wide = operand1->isWide ();

  if (wide && operand1->isAccumulator () && operand2->isRegister ()) {
    outstream->writeByte (0x90 | operand2->value ());
  } else {
    outstream->writeByte (0x86 | wide);
    writeModrm (outstream, operand1->value (), operand2);
  }

  ___CBTPOP;
}

void compiler_t::writeDisplacement (asmWriter_t* outstream, operand_t* memOperand) {
  ___CBTPUSH;

  switch (memOperand->value () >> 6) {
  case 0:
    if ((memOperand->value () & 7) == 6) {
      outstream->writeWord (memOperand->displacement ());
    }
    break;
  case 1: outstream->writeByte (memOperand->displacement ()); break;
  case 2: outstream->writeWord (memOperand->displacement ());
  }

  ___CBTPOP;
}

void compiler_t::writeHeader (asmWriter_t* outstream, programme_t* programme, int interruptNumber) {
  ___CBTPUSH;

  outstream->writeByte ('X');
  outstream->writeByte ('F');
  outstream->writeWord (10); /* Size of header */
  outstream->writeWord (interruptNumber); /* Service number or FFFF */
  outstream->writeWord (programme->ipStart ()); /* IP start */
  outstream->writeWord (programme->csStart ()); /* CS start */

  ___CBTPOP;
}

void compiler_t::writeModrm (asmWriter_t* outstream, int regOrOpCode, operand_t* regOrMemOperand) {
  ___CBTPUSH;

  int modrm = regOrOpCode << 3 | regOrMemOperand->value ();
  if (regOrMemOperand->isPointer ()) {
    outstream->writeByte (modrm);
    writeDisplacement (outstream, regOrMemOperand);
  } else {
    modrm |= 0xC0;
    outstream->writeByte (modrm);
  }

  ___CBTPOP;
}
