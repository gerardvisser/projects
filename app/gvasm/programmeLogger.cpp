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
#include "data.h"
#include "instruction.h"
#include "instructionCodes.h"
#include "programmeLogger.h"

static const char* const htmlEnd = "\n    </pre>\n  </body>\n</html>\n";
static const char* const htmlStart = "<!DOCTYPE html>\n<html>\n  <head>\n    <style type=\"text/css\">\n      body {\n        background-color: #000000;\n        color: #A0A0A0;\n      }\n      pre {\n        font-size: 15px;\n      }\n      span.green {\n        color: #90C039;\n      }\n    </style>\n    <title>Programme log</title>\n  </head>\n  <body>\n    <pre>";

static void writeAam (fileOutputStream_t* stream, instruction_t* instruction);
static void writeAdd (fileOutputStream_t* stream, instruction_t* instruction);
static void writeCall (fileOutputStream_t* stream, instruction_t* instruction);
static void writeConditionalJump (fileOutputStream_t* stream, instruction_t* instruction);
static void writeData (fileOutputStream_t* stream, data_t* data);
static void writeIn (fileOutputStream_t* stream, instruction_t* instruction);
static void writeInc (fileOutputStream_t* stream, instruction_t* instruction);
static void writeInt (fileOutputStream_t* stream, instruction_t* instruction);
static void writeLea (fileOutputStream_t* stream, instruction_t* instruction);
static void writeMemOperand (fileOutputStream_t* stream, operand_t* operand);
static void writeMisc (fileOutputStream_t* stream, instruction_t* instruction);
static void writeMov (fileOutputStream_t* stream, instruction_t* instruction);
static void writeNot (fileOutputStream_t* stream, instruction_t* instruction);
static void writeOperand (fileOutputStream_t* stream, operand_t* operand, bool specifySize = false);
static void writeOperands (fileOutputStream_t* stream, instruction_t* instruction, bool alwaysSpecifySize = false);
static void writeOut (fileOutputStream_t* stream, instruction_t* instruction);
static void writePush (fileOutputStream_t* stream, instruction_t* instruction);
static void writeRet (fileOutputStream_t* stream, instruction_t* instruction);
static void writeShift (fileOutputStream_t* stream, instruction_t* instruction);
static void writeTest (fileOutputStream_t* stream, instruction_t* instruction);
static void writeXchg (fileOutputStream_t* stream, instruction_t* instruction);

void programmeLogger_write (fileOutputStream_t* htmlStream, programme_t* programme) {
  ___BTPUSH;

  htmlStream->write (htmlStart);
  htmlStream->write ("\n Loc   Size   Address");

  char buffer[256];
  int absLocation = 0;
  const int cs = programme->csStart ();
  const int codeOffset = (cs << 4) + programme->ipStart ();

  arrayList_t* byteBlocks = programme->byteBlocks ();
  const int blockCount = byteBlocks->length ();

  for (int i = 0; i < blockCount; ++i) {
    byteBlock_t* block = (byteBlock_t*) byteBlocks->get (i);
    if (absLocation >= codeOffset) {
      sprintf (buffer, "\n%05X%6X  <span class=\"green\">%04X:%04X</span>  ", absLocation, block->size (), cs, block->location ());
    } else {
      sprintf (buffer, "\n%05X%6X  0000:%04X  ", absLocation, block->size (), block->location ());
    }
    htmlStream->write (buffer);
    absLocation += block->size ();

    switch (block->type ()) {
    case byteBlock_t::TYPE_DATA:
      writeData (htmlStream, (data_t*) block);
      break;

    case byteBlock_t::TYPE_INSTRUCTION:
      switch (((instruction_t*) block)->group ()) {
      case GRP_AAM: writeAam (htmlStream, (instruction_t*) block); break;
      case GRP_ADD: writeAdd (htmlStream, (instruction_t*) block); break;
      case GRP_CALL:
      case GRP_JMP: writeCall (htmlStream, (instruction_t*) block); break;
      case GRP_CONJMP: writeConditionalJump (htmlStream, (instruction_t*) block); break;
      case GRP_IN: writeIn (htmlStream, (instruction_t*) block); break;
      case GRP_INC: writeInc (htmlStream, (instruction_t*) block); break;
      case GRP_INT: writeInt (htmlStream, (instruction_t*) block); break;
      case GRP_LEA: writeLea (htmlStream, (instruction_t*) block); break;
      case GRP_MISC: writeMisc (htmlStream, (instruction_t*) block); break;
      case GRP_MOV: writeMov (htmlStream, (instruction_t*) block); break;
      case GRP_NOT: writeNot (htmlStream, (instruction_t*) block); break;
      case GRP_OUT: writeOut (htmlStream, (instruction_t*) block); break;
      case GRP_PUSH: writePush (htmlStream, (instruction_t*) block); break;
      case GRP_RET: writeRet (htmlStream, (instruction_t*) block); break;
      case GRP_SHIFT: writeShift (htmlStream, (instruction_t*) block); break;
      case GRP_TEST: writeTest (htmlStream, (instruction_t*) block); break;
      case GRP_XCHG: writeXchg (htmlStream, (instruction_t*) block);
      }
      break;
    }
  }

  htmlStream->write (htmlEnd);

  ___BTPOP;
}

static void writeAam (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  switch (instruction->operationCode ()) {
  case 0xD4: stream->write ("AAM"); break;
  case 0xD5: stream->write ("AAD");
  }
  operand_t* operand = instruction->operand1 ();
  if (operand != NULL) {
    stream->write ("     ");
    writeOperand (stream, operand);
  }

  ___BTPOP;
}

static void writeAdd (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  switch (instruction->operationCode ()) {
  case 0: stream->write ("ADD     "); break;
  case 1: stream->write ("OR      "); break;
  case 2: stream->write ("ADC     "); break;
  case 3: stream->write ("SBB     "); break;
  case 4: stream->write ("AND     "); break;
  case 5: stream->write ("SUB     "); break;
  case 6: stream->write ("XOR     "); break;
  case 7: stream->write ("CMP     ");
  }
  writeOperands (stream, instruction);

  ___BTPOP;
}

static void writeCall (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  operand_t* operand1 = instruction->operand1 ();
  operand_t* operand2 = instruction->operand2 ();

  switch (instruction->operationCode ()) {
  case 2: stream->write ("CALL    "); break;
  case 3: stream->write ("CALL    FAR "); break;
  case 4: stream->write ("JMP     "); break;
  case 5: stream->write ("JMP     FAR ");
  }
  writeOperand (stream, operand1);
  if (operand2 != NULL) {
    stream->write (':');
    writeOperand (stream, operand2);
  }

  ___BTPOP;
}

static void writeConditionalJump (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  switch (instruction->operationCode ()) {
  case 0x70: stream->write ("JO      "); break;
  case 0x71: stream->write ("JNO     "); break;
  case 0x72: stream->write ("JB      "); break;
  case 0x73: stream->write ("JNB     "); break;
  case 0x74: stream->write ("JZ      "); break;
  case 0x75: stream->write ("JNZ     "); break;
  case 0x76: stream->write ("JBE     "); break;
  case 0x77: stream->write ("JA      "); break;
  case 0x78: stream->write ("JS      "); break;
  case 0x79: stream->write ("JNS     "); break;
  case 0x7A: stream->write ("JPE     "); break;
  case 0x7B: stream->write ("JPO     "); break;
  case 0x7C: stream->write ("JL      "); break;
  case 0x7D: stream->write ("JGE     "); break;
  case 0x7E: stream->write ("JLE     "); break;
  case 0x7F: stream->write ("JG      "); break;
  case 0xE0: stream->write ("LOOPNZ  "); break;
  case 0xE1: stream->write ("LOOPZ   "); break;
  case 0xE2: stream->write ("LOOP    "); break;
  case 0xE3: stream->write ("JCXZ    ");
  }
  writeOperand (stream, instruction->operand1 ());

  ___BTPOP;
}

static void writeData (fileOutputStream_t* stream, data_t* data) {
  ___BTPUSH;

  stream->write ("data");

  ___BTPOP;
}

static void writeIn (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  stream->write ("IN      ");
  writeOperands (stream, instruction);

  ___BTPOP;
}

static void writeInc (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  switch (instruction->operationCode ()) {
  case 0: stream->write ("INC     "); break;
  case 1: stream->write ("DEC     ");
  }
  writeOperand (stream, instruction->operand1 (), true);

  ___BTPOP;
}

static void writeInt (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  stream->write ("INT     ");
  writeOperand (stream, instruction->operand1 ());

  ___BTPOP;
}

static void writeLea (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  switch (instruction->operationCode ()) {
  case 0x8D: stream->write ("LEA     "); break;
  case 0xC4: stream->write ("LES     "); break;
  case 0xC5: stream->write ("LDS     ");
  }
  writeOperands (stream, instruction);

  ___BTPOP;
}

static void writeMemOperand (fileOutputStream_t* stream, operand_t* operand) {
  ___BTPUSH;

  char buffer[16];
  int id = operand->value () & 7;
  int mod = operand->value () >> 6;
  int disp = operand->displacement () & 0xFFFF;
  if (!(mod == 0 && id == 6)) {
    switch (id) {
    case 0: stream->write ("[BX+SI"); break;
    case 1: stream->write ("[BX+DI"); break;
    case 2: stream->write ("[BP+SI"); break;
    case 3: stream->write ("[BP+DI"); break;
    case 4: stream->write ("[SI"); break;
    case 5: stream->write ("[DI"); break;
    case 6: stream->write ("[BP"); break;
    case 7: stream->write ("[BX");
    }
    if (disp != 0) {
      if (disp > 0x7FFF) {
        stream->write ('-');
        disp ^= 0xFFFF;
        ++disp;
      } else {
        stream->write ('+');
      }
      sprintf (buffer, "%X", disp);
      stream->write (buffer);
    }
    stream->write (']');
  } else {
    sprintf (buffer, "[%04X]", disp);
    stream->write (buffer);
  }

  ___BTPOP;
}

static void writeMisc (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  switch (instruction->operationCode ()) {
  case 0x26: stream->write ("ES:"); break;
  case 0x27: stream->write ("DAA"); break;
  case 0x2E: stream->write ("CS:"); break;
  case 0x2F: stream->write ("DAS"); break;
  case 0x36: stream->write ("SS:"); break;
  case 0x37: stream->write ("AAA"); break;
  case 0x3E: stream->write ("DS:"); break;
  case 0x3F: stream->write ("AAS"); break;
  case 0x90: stream->write ("NOP"); break;
  case 0x98: stream->write ("CBW"); break;
  case 0x99: stream->write ("CWD"); break;
  case 0x9B: stream->write ("WAIT"); break;
  case 0x9C: stream->write ("PUSHF"); break;
  case 0x9D: stream->write ("POPF"); break;
  case 0x9E: stream->write ("SAHF"); break;
  case 0x9F: stream->write ("LAHF"); break;
  case 0xA4: stream->write ("MOVSB"); break;
  case 0xA5: stream->write ("MOVSW"); break;
  case 0xA6: stream->write ("CMPSB"); break;
  case 0xA7: stream->write ("CMPSW"); break;
  case 0xAA: stream->write ("STOSB"); break;
  case 0xAB: stream->write ("STOSW"); break;
  case 0xAC: stream->write ("LODSB"); break;
  case 0xAD: stream->write ("LODSW"); break;
  case 0xAE: stream->write ("SCASB"); break;
  case 0xAF: stream->write ("SCASW"); break;
  case 0xCE: stream->write ("INTO"); break;
  case 0xCF: stream->write ("IRET"); break;
  case 0xD7: stream->write ("XLAT"); break;
  case 0xF0: stream->write ("LOCK"); break;
  case 0xF2: stream->write ("REPNZ"); break;
  case 0xF3: stream->write ("REPZ"); break;
  case 0xF4: stream->write ("HLT"); break;
  case 0xF5: stream->write ("CMC"); break;
  case 0xF8: stream->write ("CLC"); break;
  case 0xF9: stream->write ("STC"); break;
  case 0xFA: stream->write ("CLI"); break;
  case 0xFB: stream->write ("STI"); break;
  case 0xFC: stream->write ("CLD"); break;
  case 0xFD: stream->write ("STD"); break;
  }

  ___BTPOP;
}

static void writeMov (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  stream->write ("MOV     ");
  writeOperands (stream, instruction);

  ___BTPOP;
}

static void writeNot (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  switch (instruction->operationCode ()) {
  case 2: stream->write ("NOT     "); break;
  case 3: stream->write ("NEG     "); break;
  case 4: stream->write ("MUL     "); break;
  case 5: stream->write ("IMUL    "); break;
  case 6: stream->write ("DIV     "); break;
  case 7: stream->write ("IDIV    ");
  }
  writeOperand (stream, instruction->operand1 (), true);

  ___BTPOP;
}

static void writeOperand (fileOutputStream_t* stream, operand_t* operand, bool specifySize) {
  ___BTPUSH;

  char buffer[16];
  if (operand->isWide ()) {
    switch (operand->type ()) {
    case OPERAND_TYPE_IMMEDIATE:
      sprintf (buffer, "%04X", operand->value () & 0xFFFF);
      stream->write (buffer);
      break;

    case OPERAND_TYPE_POINTER:
      if (specifySize) {
        stream->write ("WORD PTR ");
      }
      writeMemOperand (stream, operand);
      break;

    case OPERAND_TYPE_REGISTER:
      switch (operand->value ()) {
      case 0: stream->write ("AX"); break;
      case 1: stream->write ("CX"); break;
      case 2: stream->write ("DX"); break;
      case 3: stream->write ("BX"); break;
      case 4: stream->write ("SP"); break;
      case 5: stream->write ("BP"); break;
      case 6: stream->write ("SI"); break;
      case 7: stream->write ("DI");
      }
      break;

    case OPERAND_TYPE_SEGREGISTER:
      switch (operand->value ()) {
      case 0: stream->write ("ES"); break;
      case 1: stream->write ("CS"); break;
      case 2: stream->write ("SS"); break;
      case 3: stream->write ("DS");
      }
      break;
    }
  } else {
    switch (operand->type ()) {
    case OPERAND_TYPE_IMMEDIATE:
      sprintf (buffer, "%02X", operand->value () & 0xFF);
      stream->write (buffer);
      break;

    case OPERAND_TYPE_POINTER:
      if (specifySize) {
        stream->write ("BYTE PTR ");
      }
      writeMemOperand (stream, operand);
      break;

    case OPERAND_TYPE_REGISTER:
      switch (operand->value ()) {
      case 0: stream->write ("AL"); break;
      case 1: stream->write ("CL"); break;
      case 2: stream->write ("DL"); break;
      case 3: stream->write ("BL"); break;
      case 4: stream->write ("AH"); break;
      case 5: stream->write ("CH"); break;
      case 6: stream->write ("DH"); break;
      case 7: stream->write ("BH");
      }
      break;
    }
  }

  ___BTPOP;
}

static void writeOperands (fileOutputStream_t* stream, instruction_t* instruction, bool alwaysSpecifySize) {
  ___BTPUSH;

  operand_t* operand1 = instruction->operand1 ();
  operand_t* operand2 = instruction->operand2 ();
  writeOperand (stream, operand1, operand2->isImmediate () | alwaysSpecifySize);
  stream->write (", ");
  writeOperand (stream, operand2);

  ___BTPOP;
}

static void writeOut (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  stream->write ("OUT     ");
  writeOperands (stream, instruction);

  ___BTPOP;
}

static void writePush (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  switch (instruction->operationCode ()) {
  case 0: stream->write ("PUSH    "); break;
  case 1: stream->write ("POP     ");
  }
  writeOperand (stream, instruction->operand1 ());

  ___BTPOP;
}

static void writeRet (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  const char* padding;
  switch (instruction->operationCode ()) {
  case 0xC2:
    stream->write ("RET");
    padding = "     ";
    break;

  case 0xCA:
    stream->write ("RETF");
    padding = "    ";
  }
  operand_t* operand = instruction->operand1 ();
  if (operand != NULL) {
    stream->write (padding);
    writeOperand (stream, operand);
  }

  ___BTPOP;
}

static void writeShift (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  switch (instruction->operationCode ()) {
  case 0: stream->write ("ROL     "); break;
  case 1: stream->write ("ROR     "); break;
  case 2: stream->write ("RCL     "); break;
  case 3: stream->write ("RCR     "); break;
  case 4: stream->write ("SHL     "); break;
  case 5: stream->write ("SHR     "); break;
  case 6: stream->write ("SAL     "); break;
  case 7: stream->write ("SAR     ");
  }
  writeOperands (stream, instruction, true);

  ___BTPOP;
}

static void writeTest (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  stream->write ("TEST     ");
  writeOperands (stream, instruction);

  ___BTPOP;
}

static void writeXchg (fileOutputStream_t* stream, instruction_t* instruction) {
  ___BTPUSH;

  stream->write ("XCHG    ");
  writeOperands (stream, instruction);

  ___BTPOP;
}
