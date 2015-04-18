/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2014, 2015 Gerard Visser.

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
#include "printer.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

REFL_IMPL (printer_t, object_t)

#define AL 8
#define CL 9
#define DL 10
#define BL 11
#define AH 12
#define CH 13
#define DH 14
#define BH 15

class instruction_t {
private:
  char str[48];
  int stridx;

public:
  const unsigned char* const bytes;
  int size;
  bool illegal;
  bool prefix;

  instruction_t (const unsigned char* bytes) : bytes (bytes), stridx (0), illegal (false), prefix (false) {
    str[0] = 0;
  }

  void append (char c) {
    str[stridx] = c;
    ++stridx;
    str[stridx] = 0;
  }

  void append (const char* templ, ...) {
    va_list ap;
    va_start (ap, templ);
    stridx += vsprintf (str + stridx, templ, ap);
    va_end (ap);
  }

  void appendSignedByte (signed char val) {
    if (val < 0) {
      val = -val;
      append ('-');
    } else {
      append ('+');
    }
    append ("%02X", val);
  }

  const char* asText (void) {
    return str;
  }

  int textLength (void) {
    return stridx;
  }
};

static void printAam (instruction_t* instr, int maxInstrLength);
static void printAddAccImm (instruction_t* instr, int maxInstrLength);
static void printAddRmImm (instruction_t* instr, int maxInstrLength);
static void printAddRmReg (instruction_t* instr, int maxInstrLength);
static void printCallDisp (instruction_t* instr, int maxInstrLength, unsigned short instrOffset);
static void printCallImmImm (instruction_t* instr, int maxInstrLength);
static void printCBW (instruction_t* instr);
static void printConditionalJump (instruction_t* instr, int maxInstrLength, unsigned short instrOffset);
static void printCoprocessorInstruction (instruction_t* instr, int maxInstrLength);
static void printDB (instruction_t* instr);
static void printEnter (instruction_t* instr, int maxInstrLength);
static void printExtraInstruction (instruction_t* instr, int maxInstrLength);
static void printIncRm (instruction_t* instr, int maxInstrLength);
static void printInOut (instruction_t* instr, int maxInstrLength);
static void printInstructionBytes (instruction_t* instr);
static void printInterrupt (instruction_t* instr, int maxInstrLength);
static void printLea (instruction_t* instr, int maxInstrLength);
static void printLes (instruction_t* instr, int maxInstrLength);
static void printLock (instruction_t* instr);
static void printLoop (instruction_t* instr, int maxInstrLength, unsigned short instrOffset);
static void printMovAccMem (instruction_t* instr, int maxInstrLength);
static void printMovMemAcc (instruction_t* instr, int maxInstrLength);
static void printMovRegImm (instruction_t* instr, int maxInstrLength);
static void printMovRmImm (instruction_t* instr, int maxInstrLength);
static void printMovRmReg (instruction_t* instr, int maxInstrLength);
static void printMovRmSeg (instruction_t* instr, int maxInstrLength);
static void printNotRm (instruction_t* instr, int maxInstrLength);
static void printPopRm (instruction_t* instr, int maxInstrLength);
static void printPushReg (instruction_t* instr);
static void printPushSeg (instruction_t* instr);
static void printRet (instruction_t* instr, int maxInstrLength);
static void printShift (instruction_t* instr, int maxInstrLength);
static void printShiftImm (instruction_t* instr, int maxInstrLength);
static void printSpaces (int total);
static void printStringInstruction (instruction_t* instr);
static void printTestAccImm (instruction_t* instr, int maxInstrLength);
static void printTestRmReg (instruction_t* instr, int maxInstrLength);
static void printXchgAxReg (instruction_t* instr);

printer_t::printer_t (registers_t* registers, memory_t* memory) : registers (registers), memory (memory) {
  ___CBTPUSH;

  registers->reserve ();
  memory->reserve ();

  ___CBTPOP;
}

printer_t::~printer_t (void) {
  ___CBTPUSH;

  registers->dump ();
  memory->dump ();

  ___CBTPOP;
}

void printer_t::printCode (address_t* address, int maxBytes) {
  ___CBTPUSH;

  while (maxBytes > 0) {
    maxBytes -= printInstruction (address);
  }

  ___CBTPOP;
}

int printer_t::printInstruction (address_t* address) {
  ___CBTPUSH;

  unsigned char buf[6];
  int maxInstrLength = 0x10000 - address->offset ();

  memory->readBytes (buf, address->value (), 6);
  instruction_t instr (buf);

  switch (buf[0] >> 6) {
  case 0: /* 00 - 3F */
    switch (buf[0] & 6) {
    case 6: printPushSeg (&instr); break;
    case 4: printAddAccImm (&instr, maxInstrLength); break;
    default: printAddRmReg (&instr, maxInstrLength);
    }
    break;

  case 1: /* 40 - 7F */
    switch (buf[0] & 0x30) {
    case 0x20: printExtraInstruction (&instr, maxInstrLength); break;
    case 0x30: printConditionalJump (&instr, maxInstrLength, address->offset ()); break;
    default: printPushReg (&instr);
    }
    break;

  case 2: /* 80 - BF */
    switch (buf[0] & 0x30) {
    case 0: /* 80 - 8F */
      switch (buf[0] & 0xC) {
      case 0: printAddRmImm (&instr, maxInstrLength); break;
      case 4: printTestRmReg (&instr, maxInstrLength); break;
      case 8: printMovRmReg (&instr, maxInstrLength); break;
      default:
        if ((buf[0] & 1) == 0) {
          printMovRmSeg (&instr, maxInstrLength);
        } else if ((buf[0] & 2) == 0) {
          printLea (&instr, maxInstrLength);
        } else {
          printPopRm (&instr, maxInstrLength);
        }
      }
      break;

    case 0x10: /* 90 - 9F */
      if (buf[0] < 0x98) {
        printXchgAxReg (&instr);
      } else if (buf[0] == 0x9A) {
        printCallImmImm (&instr, maxInstrLength);
      } else {
        printCBW (&instr);
      }
      break;

    case 0x20: /* A0 - AF */
      switch (buf[0] & 0xE) {
      case 0: printMovAccMem (&instr, maxInstrLength); break;
      case 2: printMovMemAcc (&instr, maxInstrLength); break;
      case 8: printTestAccImm (&instr, maxInstrLength); break;
      default: printStringInstruction (&instr);
      }
      break;

    case 0x30: /* B0 - BF */
      printMovRegImm (&instr, maxInstrLength);
    }
    break;

  case 3: /* C0 - FF */
    switch (buf[0] & 0x30) {
    case 0: /* C0 - CF */
      switch (buf[0] & 0xE) {
      case 0: printShiftImm (&instr, maxInstrLength); break;
      case 2:
      case 10: printRet (&instr, maxInstrLength); break;
      case 4: printLes (&instr, maxInstrLength); break;
      case 6: printMovRmImm (&instr, maxInstrLength); break;
      case 8: printEnter (&instr, maxInstrLength); break;
      case 12:
      case 14: printInterrupt (&instr, maxInstrLength);
      }
      break;

    case 0x10: /* D0 - DF */
      switch (buf[0] & 0xC) {
      case 0: printShift (&instr, maxInstrLength); break;
      case 4: printAam (&instr, maxInstrLength); break;
      default: printCoprocessorInstruction (&instr, maxInstrLength);
      }
      break;

    case 0x20: /* E0 - EF */
      if ((buf[0] & 4) != 0) {
        printInOut (&instr, maxInstrLength);
      } else if (buf[0] < 0xE4 || buf[0] == 0xEB) {
        printLoop (&instr, maxInstrLength, address->offset ());
      } else if (buf[0] < 0xEA) {
        printCallDisp (&instr, maxInstrLength, address->offset ());
      } else {
        printCallImmImm (&instr, maxInstrLength);
      }
      break;

    case 0x30: /* F0 - FF */
      switch (buf[0] & 0xE) {
      case 6:
        if ((buf[1] & 0x38) == 0) {
          printMovRmImm (&instr, maxInstrLength);
        } else {
          printNotRm (&instr, maxInstrLength);
        }
        break;
      case 14: printIncRm (&instr, maxInstrLength); break;
      default: printLock (&instr);
      }
    }
  }

  printf ("%04X:%04X ", address->segment (), address->offset ());
  if (instr.illegal) {
    printf ("\x1B[31m");
  }
  printInstructionBytes (&instr);
  printf ("%s", instr.asText ());
  if (instr.illegal) {
    printf ("\x1B[0m");
  }
  printf ("\n");

  address->add (instr.size);
  if (instr.prefix) {
    instr.size += printInstruction (address);
  }

  ___CBTPOP;
  return instr.size;
}

int printer_t::printMemLine (address_t* address, int maxBytes) {
  ___CBTPUSH;

  char ascii[17];
  unsigned char bytes[16];
  const int r = address->offset () & 0xF;

  printf ("%04X:%04X  ", address->segment (), address->offset () & 0xFFF0);
  printSpaces (3 * r);
  memset (ascii, ' ', r);
  if (maxBytes > 16 - r) {
    maxBytes = 16 - r;
  }
  memory->readBytes (bytes, address->value (), maxBytes);
  for (int i = 0; i < maxBytes; ++i) {
    printf ("%02X", bytes[i]);
    ascii[i + r] = bytes[i] < 0x20 || bytes[i] > 0x7E ? '.' : bytes[i];
    if (i + r == 7 && maxBytes + r != 8) {
      putchar ('-');
    } else {
      putchar (' ');
    }
  }
  printSpaces (3 * (16 - maxBytes - r));
  ascii[maxBytes + r] = 0;
  printf ("  %s\n", ascii);

  address->add (maxBytes);

  ___CBTPOP;
  return maxBytes;
}

void printer_t::printMemory (address_t* address, int maxBytes) {
  ___CBTPUSH;

  while (maxBytes > 0) {
    maxBytes -= printMemLine (address, maxBytes);
  }

  ___CBTPOP;
}

void printer_t::printRegisters (void) {
  ___CBTPUSH;

  printf ("AX=%04X  BX=%04X  CX=%04X  DX=%04X  ", registers->ax (), registers->bx (), registers->cx (), registers->dx ());
  printf ("SP=%04X  BP=%04X  SI=%04X  DI=%04X\n", registers->sp (), registers->bp (), registers->si (), registers->di ());
  printf ("DS=%04X  ES=%04X  SS=%04X  CS=%04X  IP=%04X  ", registers->ds (), registers->es (), registers->ss (), registers->cs (), registers->ip ());
  if ((registers->flags () & F_OVERFLOW) != 0) printf (" OV"); else printf (" NV");
  if ((registers->flags () & F_DIRECTION) != 0) printf (" DN"); else printf (" UP");
  if ((registers->flags () & F_INTERRUPT) != 0) printf (" EI"); else printf (" DI");
  if ((registers->flags () & F_SIGN) != 0) printf (" NG"); else printf (" PL");
  if ((registers->flags () & F_ZERO) != 0) printf (" ZR"); else printf (" NZ");
  if ((registers->flags () & F_ADJUST) != 0) printf (" AC"); else printf (" NA");
  if ((registers->flags () & F_PARITY) != 0) printf (" PE"); else printf (" PO");
  if ((registers->flags () & F_CARRY) != 0) printf (" CY\n"); else printf (" NC\n");
  address_t address (registers->cs (), registers->ip ());
  printInstruction (&address);

  ___CBTPOP;
}

static void printMiscCoprocessorInstruction (instruction_t* instr);
static void printModrm (instruction_t* instr);
static void printModrm (instruction_t* instr, const bool dstRrr, const bool wide);
static void printModrmPointer (instruction_t* instr, const bool wide);
static void printPointerType (instruction_t* instr);
static int readWord (const unsigned char* buf, int offset);

static const char* getAddGroupInstructionName (int idByte) {
  switch (idByte >> 3 & 7) {
  case OPC_ADD: return "ADD     ";
  case OPC_OR: return "OR      ";
  case OPC_ADC: return "ADC     ";
  case OPC_SBB: return "SBB     ";
  case OPC_AND: return "AND     ";
  case OPC_SUB: return "SUB     ";
  case OPC_XOR: return "XOR     ";
  case OPC_CMP: return "CMP     ";
  }
}

static const char* getFaddGroupInstructionName (int idByte) {
  switch (idByte >> 3 & 7) {
  case 0: return "FADD    ";
  case 1: return "FMUL    ";
  case 2: return "FCOM    ";
  case 3: return "FCOMP   ";
  case 4: return "FSUB    ";
  case 5: return "FSUBR   ";
  case 6: return "FDIV    ";
  case 7: return "FDIVR   ";
  }
}

static const char* getFaddpGroupInstructionName (int idByte) {
  switch (idByte >> 3 & 7) {
  case 0: return "FADDP   ";
  case 1: return "FMULP   ";
  case 2: return "FCOMP   "; /*!!!*/
  case 3: return "FCOMPP  "; /*!!*/
  case 4: return "FSUBRP  ";
  case 5: return "FSUBP   ";
  case 6: return "FDIVRP  ";
  case 7: return "FDIVP   ";
  }
}

static const char* getFiaddGroupInstructionName (int idByte) {
  switch (idByte >> 3 & 7) {
  case 0: return "FIADD   ";
  case 1: return "FIMUL   ";
  case 2: return "FICOM   ";
  case 3: return "FICOMP  ";
  case 4: return "FISUB   ";
  case 5: return "FISUBR  ";
  case 6: return "FIDIV   ";
  case 7: return "FIDIVR  ";
  }
}

static const char* getFildGroupInstructionName (int idByte) {
  switch (idByte >> 3 & 3) {
  case 0: return "FILD    ";
  case 1: return "???     ";
  case 2: return "FIST    ";
  case 3: return "FISTP   ";
  }
}

static const char* getFldenvGroupInstructionName (int idByte) {
  switch (idByte >> 3 & 3) {
  case 0: return "FLDENV  ";
  case 1: return "FLDCW   ";
  case 2: return "FSTENV  ";
  case 3: return "FSTCW   ";
  }
}

static const char* getFldGroupInstructionName (int idByte) {
  switch (idByte >> 3 & 3) {
  case 0: return "FLD     ";
  case 1: return "???     ";
  case 2: return "FST     ";
  case 3: return "FSTP    ";
  }
}

static const char* getFrstorGroupInstructionName (int idByte) {
  switch (idByte >> 3 & 3) {
  case 0: return "FRSTOR  ";
  case 1: return "???     ";
  case 2: return "FSAVE   ";
  case 3: return "FSTSW   ";
  }
}

static const char* getIncGroupInstructionName (int idByte) {
  switch (idByte >> 3 & 7) {
  case 0: return "INC     ";
  case 1: return "DEC     ";
  case 2: return "CALL    ";
  case 3: return "CALL    FAR ";
  case 4: return "JMP     ";
  case 5: return "JMP     FAR ";
  case 6: return "PUSH    ";
  default: return "???     ";
  }
}

static const char* getModrmPointerPart (int id) {
  switch (id) {
  case 0: return "[BX+SI";
  case 1: return "[BX+DI";
  case 2: return "[BP+SI";
  case 3: return "[BP+DI";
  case 4: return "[SI";
  case 5: return "[DI";
  case 6: return "[BP";
  case 7: return "[BX";
  }
}

static const char* getNotGroupInstructionName (int idByte) {
  switch (idByte >> 3 & 7) {
  case OPC_NOT: return "NOT     ";
  case OPC_NEG: return "NEG     ";
  case OPC_MUL: return "MUL     ";
  case OPC_IMUL: return "IMUL    ";
  case OPC_DIV: return "DIV     ";
  case OPC_IDIV: return "IDIV    ";
  default: return "???     ";
  }
}

static const char* getReg (int id, bool wide) {
  switch (!wide << 3 | id) {
  case AX: return "AX";
  case CX: return "CX";
  case DX: return "DX";
  case BX: return "BX";
  case SP: return "SP";
  case BP: return "BP";
  case SI: return "SI";
  case DI: return "DI";
  case AL: return "AL";
  case CL: return "CL";
  case DL: return "DL";
  case BL: return "BL";
  case AH: return "AH";
  case CH: return "CH";
  case DH: return "DH";
  case BH: return "BH";
  }
}

static const char* getSeg (int idByte) {
  switch (idByte >> 3 & 3) {
  case ES: return "ES";
  case CS: return "CS";
  case SS: return "SS";
  case DS: return "DS";
  }
}

static const char* getShiftInstructionName (int idByte) {
  switch (idByte >> 3 & 7) {
  case OPC_ROL: return "ROL     ";
  case OPC_ROR: return "ROR     ";
  case OPC_RCL: return "RCL     ";
  case OPC_RCR: return "RCR     ";
  case OPC_SHL: return "SHL     ";
  case OPC_SHR: return "SHR     ";
  case OPC_SAL: return "SAL     ";
  case OPC_SAR: return "SAR     ";
  }
}

static int modrmExtraLength (int modrm) {
  ___BTPUSH;

  int result = modrm >> 6;
  switch (result) {
  case 0:
    result = ((modrm & 7) == 6) << 1;
    break;

  case 3:
    result = 0;
  }

  ___BTPOP;
  return result;
}

static void printAam (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 1 + ((instr->bytes[0] & 2) == 0);
  if (instr->size <= maxInstrLength && instr->bytes[0] != 0xD6) {
    if (instr->size == 2) {
      if (instr->bytes[0] & 1) {
        instr->append ("AAD");
      } else {
        instr->append ("AAM");
      }
      if (instr->bytes[1] != 10) {
        instr->append ("     %02X", instr->bytes[1]);
      }
    } else {
      instr->append ("XLAT");
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printAddAccImm (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  bool wide = (instr->bytes[0] & 1) != 0;
  instr->size = 2 + wide;
  if (instr->size <= maxInstrLength) {
    instr->append (getAddGroupInstructionName (instr->bytes[0]));
    if (wide) {
      instr->append ("AX,%04X", readWord (instr->bytes, 1));
    } else {
      instr->append ("AL,%02X", instr->bytes[1]);
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printAddRmImm (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  const bool wide = instr->bytes[0] & 1;
  const int dispLen = modrmExtraLength (instr->bytes[1]);
  const int immDataLength = 2 - ((instr->bytes[0] & 2) != 0 || !wide);
  instr->size = 2 + immDataLength + dispLen;
  if (instr->size <= maxInstrLength) {
    instr->append (getAddGroupInstructionName (instr->bytes[1]));
    printPointerType (instr);
    printModrmPointer (instr, wide);
    instr->append (',');
    if (immDataLength == 2) {
      instr->append ("%04X", readWord (instr->bytes, 2 + dispLen));
    } else if (wide) {
      instr->appendSignedByte (instr->bytes[2 + dispLen]);
    } else {
      instr->append ("%02X", instr->bytes[2 + dispLen]);
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printAddRmReg (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    instr->append (getAddGroupInstructionName (instr->bytes[0]));
    printModrm (instr);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printCallDisp (instruction_t* instr, int maxInstrLength, unsigned short instrOffset) {
  ___BTPUSH;

  if (maxInstrLength > 2) {
    instr->size = 3;
    if (instr->bytes[0] & 1) {
      instr->append ("JMP     ");
    } else {
      instr->append ("CALL    ");
    }
    instrOffset += 3 + readWord (instr->bytes, 1);
    instr->append ("%04X", instrOffset);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printCallImmImm (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 5;
  if (instr->size <= maxInstrLength) {
    int offset = readWord (instr->bytes, 1);
    int segment = readWord (instr->bytes, 3);
    const char* name = instr->bytes[0] == 0x9A ? "CALL" : "JMP ";
    instr->append ("%s    %04X:%04X", name, segment, offset);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printCBW (instruction_t* instr) {
  ___BTPUSH;

  instr->size = 1;
  switch (instr->bytes[0] & 7) {
  case 0: instr->append ("CBW"); break;
  case 1: instr->append ("CWD"); break;
  case 3: instr->append ("WAIT"); break;
  case 4: instr->append ("PUSHF"); break;
  case 5: instr->append ("POPF"); break;
  case 6: instr->append ("SAHF"); break;
  case 7: instr->append ("LAHF");
  }

  ___BTPOP;
}

static void printConditionalJump (instruction_t* instr, int maxInstrLength, unsigned short instrOffset) {
  ___BTPUSH;

  if (maxInstrLength > 1) {
    instr->size = 2;
    switch (instr->bytes[0] & 0xF) {
    case 0: instr->append ("JO      "); break;
    case 1: instr->append ("JNO     "); break;
    case 2: instr->append ("JB      "); break;
    case 3: instr->append ("JNB     "); break;
    case 4: instr->append ("JZ      "); break;
    case 5: instr->append ("JNZ     "); break;
    case 6: instr->append ("JBE     "); break;
    case 7: instr->append ("JA      "); break;
    case 8: instr->append ("JS      "); break;
    case 9: instr->append ("JNS     "); break;
    case 10: instr->append ("JPE     "); break;
    case 11: instr->append ("JPO     "); break;
    case 12: instr->append ("JL      "); break;
    case 13: instr->append ("JGE     "); break;
    case 14: instr->append ("JLE     "); break;
    case 15: instr->append ("JG      ");
    }
    instrOffset += 2 + (signed char) instr->bytes[1];
    instr->append ("%04X", instrOffset);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printCoprocessorInstruction (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  /* 8087 instructions. */
  instr->illegal = true;

  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    instr->append ("        ");

    switch (instr->bytes[0] & 7) {
    case 0:
      instr->append (getFaddGroupInstructionName (instr->bytes[1]));
      if (instr->bytes[1] >> 6 == 3) {
        const int instrCode = instr->bytes[1] >> 3 & 7;
        if (instrCode < 2 | instrCode > 3) {
          instr->append ("ST,");
        }
        instr->append ("ST(%d)", instr->bytes[1] & 7);
      } else {
        instr->append ("DWORD PTR ");
        printModrmPointer (instr, true);
      }
      break;

    case 1:
      if (instr->bytes[1] >> 6 != 3) {
        if ((instr->bytes[1] & 0x20) == 0) {
          //instr->illegal = (instr->bytes[1] & 0x38) == 8;
          instr->append (getFldGroupInstructionName (instr->bytes[1]));
          instr->append ("DWORD PTR ");
        } else {
          instr->append (getFldenvGroupInstructionName (instr->bytes[1]));
        }
        printModrmPointer (instr, true);
      } else {
        printMiscCoprocessorInstruction (instr);
      }
      break;

    case 2:
      if (instr->bytes[1] >> 6 != 3) {
        instr->append (getFiaddGroupInstructionName (instr->bytes[1]));
        instr->append ("DWORD PTR ");
        printModrmPointer (instr, true);
      } else {
        instr->append ("???");
        instr->illegal = true;
      }
      break;

    case 3:
      if (instr->bytes[1] >> 6 != 3) {
        if ((instr->bytes[1] & 0x20) == 0) {
          //instr->illegal = (instr->bytes[1] & 0x38) == 8;
          instr->append (getFildGroupInstructionName (instr->bytes[1]));
          instr->append ("DWORD PTR ");
        } else {
          switch (instr->bytes[1] >> 3 & 3) {
          case 0: instr->append ("???     "); instr->illegal = true; break;
          case 1: instr->append ("FLD     "); break;
          case 2: instr->append ("???     "); instr->illegal = true; break;
          case 3: instr->append ("FSTP    ");
          }
          instr->append ("TBYTE PTR ");
        }
        printModrmPointer (instr, true);
      } else {
        switch (instr->bytes[1] & 0x3F) {
        case 0x20: instr->append ("FENI"); break;
        case 0x21: instr->append ("FDISI"); break;
        case 0x22: instr->append ("FCLEX"); break;
        case 0x23: instr->append ("FINIT"); break;
        case 0x24: instr->append ("FSETPM"); instr->illegal = true; /* 80287 instruction. */ break;
        default: instr->append ("???"); instr->illegal = true;
        }
      }
      break;

    case 4:
      if (instr->bytes[1] >> 6 == 3) {
        /* Here, for some reason, FSUBR precedes FSUB and FDIVR precedes FDIV! */
        int idByte = instr->bytes[1];
        if ((idByte & 0x20) != 0) {
          idByte ^= 0x08;
        }
        instr->append (getFaddGroupInstructionName (idByte));
        const int instrCode = instr->bytes[1] >> 3 & 7;
        instr->append ("ST(%d)", instr->bytes[1] & 7);
        if (instrCode < 2 | instrCode > 3) {
          instr->append (",ST");
        }
      } else {
        instr->append (getFaddGroupInstructionName (instr->bytes[1]));
        instr->append ("QWORD PTR ");
        printModrmPointer (instr, true);
      }
      break;

    case 5:
      if (instr->bytes[1] >> 6 != 3) {
        //instr->illegal = (instr->bytes[1] & 0x18) == 8;
        if ((instr->bytes[1] & 0x20) == 0) {
          instr->append (getFldGroupInstructionName (instr->bytes[1]));
          instr->append ("QWORD PTR ");
        } else {
          instr->append (getFrstorGroupInstructionName (instr->bytes[1]));
        }
        printModrmPointer (instr, true);
      } else if (instr->bytes[1] < 0xE0) {
        switch (instr->bytes[1] >> 3 & 3) {
        case 0: instr->append ("FFREE   "); break;
        case 1: instr->append ("FXCH    "); break; /* Zit ook op D9 C8 e.v.: is deze hier illegaal? */
        case 2: instr->append ("FST     "); break;
        case 3: instr->append ("FSTP    "); /* Zit ook op D9 D8 e.v.: is die daar illegaal? */
        }
        instr->append ("ST(%d)", instr->bytes[1] & 7);
      } else {
        instr->append ("???");
        instr->illegal = true;
      }
      break;

    case 6:
      if (instr->bytes[1] >> 6 != 3) {
        instr->append (getFiaddGroupInstructionName (instr->bytes[1]));
        instr->append ("WORD PTR ");
        printModrmPointer (instr, true);
      } else {
        instr->append (getFaddpGroupInstructionName (instr->bytes[1]));
        const int instrCode = instr->bytes[1] >> 3 & 7;
        instr->append ("ST(%d)", instr->bytes[1] & 7);
        if (instrCode < 2 | instrCode > 3) {
          instr->append (",ST");
        }
        /* NOOT: FCOMPP alleen geldig voor S(1);
                 Zijn de FCOMP instrcuties hier ueberhaupt geldig?? */
        //instr->illegal = instr->bytes[1] > 0xD7 & instr->bytes[1] < 0xE0 & instr->bytes[1] != 0xD9;
      }
      break;

    case 7:
      if (instr->bytes[1] >> 6 != 3) {
        if ((instr->bytes[1] & 0x20) == 0) {
          //instr->illegal = (instr->bytes[1] & 0x38) == 8;
          instr->append (getFildGroupInstructionName (instr->bytes[1]));
          instr->append ("WORD PTR ");
        } else {
          switch (instr->bytes[1] >> 3 & 3) {
          case 0: instr->append ("FBLD    TBYTE PTR "); break;
          case 1: instr->append ("FILD    QWORD PTR "); break;
          case 2: instr->append ("FBSTP   TBYTE PTR "); break;
          case 3: instr->append ("FISTP   QWORD PTR ");
          }
        }
        printModrmPointer (instr, true);
      } else if (instr->bytes[1] < 0xE0) {
        /* Al deze instructies zijn ook te vinden op DD C0 t/m DD DF: zijn deze hier illegaal? */
        switch (instr->bytes[1] >> 3 & 3) {
        case 0: instr->append ("FFREE   "); break;
        case 1: instr->append ("FXCH    "); break;
        case 2: instr->append ("FST     "); break;
        case 3: instr->append ("FSTP    ");
        }
        instr->append ("ST(%d)", instr->bytes[1] & 7);
      } else {
        instr->append ("???");
        instr->illegal = true;
      }
      break;
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printDB (instruction_t* instr) {
  ___BTPUSH;

  instr->size = 1;
  instr->append ("DB      %02X", instr->bytes[0]);
  instr->illegal = true;

  ___BTPOP;
}

static void printEnter (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  /* 80186 instructions. */
  instr->illegal = true;
  if ((instr->bytes[0] & 1) == 0) {
    instr->size = 4;
    if (instr->size <= maxInstrLength) {
      instr->append ("ENTER   %04X,%02X", readWord (instr->bytes, 1), instr->bytes[3]);
    } else {
      printDB (instr);
    }
  } else {
    instr->size = 1;
    instr->append ("LEAVE");
  }

  ___BTPOP;
}

static void printExtraInstruction (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  /* 80186 instructions. */
  instr->illegal = true;
  switch (instr->bytes[0] & 0xF) {
  case 0x0:
    instr->size = 1;
    instr->append ("PUSHA");
    break;

  case 0x1:
    instr->size = 1;
    instr->append ("POPA");
    break;

  case 0x2: {
    /* Illegal if modrm-byte specifies two registers. */
    const int dispLen = modrmExtraLength (instr->bytes[1]);
    instr->size = 2 + dispLen;
    if (instr->size <= maxInstrLength) {
      //instr->illegal = instr->bytes[1] >> 6 == 3;
      instr->append ("BOUND   ");
      printModrm (instr, true, true);
    } else {
      printDB (instr);
    }
  } break;

  case 0x8:
    instr->size = 3;
    if (instr->size <= maxInstrLength) {
      instr->append ("PUSH    %04X", readWord (instr->bytes, 1));
    } else {
      printDB (instr);
    }
    break;

  case 0x9: {
    const int dispLen = modrmExtraLength (instr->bytes[1]);
    instr->size = 4 + dispLen;
    if (instr->size <= maxInstrLength) {
      instr->append ("IMUL    ");
      printModrm (instr, true, true);
      instr->append (',');
      instr->append ("%04X", readWord (instr->bytes, 2 + dispLen));
    } else {
      printDB (instr);
    }
  } break;

  case 0xA:
    instr->size = 2;
    if (instr->size <= maxInstrLength) {
      instr->append ("PUSH    ");
      instr->appendSignedByte (instr->bytes[1]);
    } else {
      printDB (instr);
    }
    break;

  case 0xB: {
    const int dispLen = modrmExtraLength (instr->bytes[1]);
    instr->size = 3 + dispLen;
    if (instr->size <= maxInstrLength) {
      instr->append ("IMUL    ");
      printModrm (instr, true, true);
      instr->append (',');
      instr->appendSignedByte (instr->bytes[2 + dispLen]);
    } else {
      printDB (instr);
    }
  } break;

  case 0xC:
    instr->size = 1;
    instr->append ("INSB");
    break;

  case 0xD:
    instr->size = 1;
    instr->append ("INSW");
    break;

  case 0xE:
    instr->size = 1;
    instr->append ("OUTSB");
    break;

  case 0xF:
    instr->size = 1;
    instr->append ("OUTSW");
    break;

  default:
    printDB (instr);
  }

  ___BTPOP;
}

static void printIncRm (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  const bool wide = instr->bytes[0] & 1;
  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    if (wide) {
      switch (instr->bytes[1] & 0x38) {
      case 0x18:
      case 0x28: instr->illegal = instr->bytes[1] >> 6 == 3; break;
      case 0x38: instr->illegal = true;
      }
    } else {
      instr->illegal = (instr->bytes[1] & 0x38) > 8;
    }

    instr->append (getIncGroupInstructionName (instr->bytes[1]));
    if ((instr->bytes[1] & 0x38) < 0x10) {
      printPointerType (instr);
    }
    printModrmPointer (instr, wide);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printInOut (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 1 + ((instr->bytes[0] & 8) == 0);
  if (instr->size <= maxInstrLength) {
    const char* acc = instr->bytes[0] & 1 ? "AX" : "AL";
    switch (instr->bytes[0] & 0xA) {
    case 0: instr->append ("IN      %s,%02X", acc, instr->bytes[1]); break;
    case 2: instr->append ("OUT     %02X,%s", instr->bytes[1], acc); break;
    case 8: instr->append ("IN      %s,DX", acc); break;
    case 10: instr->append ("OUT     DX,%s", acc);
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printInstructionBytes (instruction_t* instr) {
  ___BTPUSH;

  int i;
  for (i = 0; i < instr->size; ++i) {
    printf ("%02X", instr->bytes[i]);
  }
  printSpaces (14 - 2*instr->size);

  ___BTPOP;
}

static void printInterrupt (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 1 + (instr->bytes[0] == 0xCD);
  if (instr->size <= maxInstrLength) {
    switch (instr->bytes[0] & 3) {
    case 0: instr->append ("INT     3"); break;
    case 1: instr->append ("INT     %02X", instr->bytes[1]); break;
    case 2: instr->append ("INTO"); break;
    case 3: instr->append ("IRET");
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printLea (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    instr->illegal = instr->bytes[1] >> 6 == 3;
    instr->append ("LEA     ");
    printModrm (instr, true, true);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printLes (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    instr->illegal = instr->bytes[1] >> 6 == 3;
    if (instr->bytes[0] & 1) {
      instr->append ("LDS     ");
    } else {
      instr->append ("LES     ");
    }
    printModrm (instr, true, true);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printLock (instruction_t* instr) {
  ___BTPUSH;

  if (instr->bytes[0] != 0xF1) {
    instr->size = 1;
    switch (instr->bytes[0] & 0xF) {
    case 0:
      instr->append ("LOCK");
      instr->prefix = true;
      break;
    case 2:
      instr->append ("REPNZ");
      instr->prefix = true;
      break;
    case 3:
      instr->append ("REPZ");
      instr->prefix = true;
      break;
    case 4: instr->append ("HLT"); break;
    case 5: instr->append ("CMC"); break;
    case 8: instr->append ("CLC"); break;
    case 9: instr->append ("STC"); break;
    case 10: instr->append ("CLI"); break;
    case 11: instr->append ("STI"); break;
    case 12: instr->append ("CLD"); break;
    case 13: instr->append ("STD");
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printLoop (instruction_t* instr, int maxInstrLength, unsigned short instrOffset) {
  ___BTPUSH;

  if (maxInstrLength > 1) {
    instr->size = 2;
    switch (instr->bytes[0] & 0xF) {
    case 0: instr->append ("LOOPNZ  "); break;
    case 1: instr->append ("LOOPZ   "); break;
    case 2: instr->append ("LOOP    "); break;
    case 3: instr->append ("JCXZ    "); break;
    case 11: instr->append ("JMP     ");
    }
    instrOffset += 2 + (signed char) instr->bytes[1];
    instr->append ("%04X", instrOffset);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printMiscCoprocessorInstruction (instruction_t* instr) {
  if (instr->bytes[1] == 0xD0) {

    instr->append ("FNOP");

  } else if (instr->bytes[1] < 0xE0) {

    switch (instr->bytes[1] >> 3 & 3) {
    case 0: instr->append ("FLD     "); break;
    case 1: instr->append ("FXCH    "); break;
    case 2: instr->append ("???     "); instr->illegal = true; break;
    case 3: instr->append ("FSTP    ");
    }
    instr->append ("ST(%d)", instr->bytes[1] & 7);

  } else {

    switch (instr->bytes[1] & 0x1F) {
    case 0x00: instr->append ("FCHS"); break;
    case 0x01: instr->append ("FABS"); break;
    case 0x04: instr->append ("FTST"); break;
    case 0x05: instr->append ("FXAM"); break;
    case 0x08: instr->append ("FLD1"); break;
    case 0x09: instr->append ("FLDL2T"); break;
    case 0x0A: instr->append ("FLDL2E"); break;
    case 0x0B: instr->append ("FLDPI"); break;
    case 0x0C: instr->append ("FLDLG2"); break;
    case 0x0D: instr->append ("FLDLN2"); break;
    case 0x0E: instr->append ("FLDZ"); break;
    case 0x10: instr->append ("F2XM1"); break;
    case 0x11: instr->append ("FYL2X"); break;
    case 0x12: instr->append ("FPTAN"); break;
    case 0x13: instr->append ("FPATAN"); break;
    case 0x14: instr->append ("FXTRACT"); break;
    case 0x15: instr->append ("FPREM1"); instr->illegal = true; /* 80387 instruction. */ break;
    case 0x16: instr->append ("FDECSTP"); break;
    case 0x17: instr->append ("FINCSTP"); break;
    case 0x18: instr->append ("FPREM"); break;
    case 0x19: instr->append ("FYL2XP1"); break;
    case 0x1A: instr->append ("FSQRT"); break;
    case 0x1B: instr->append ("FSINCOS"); instr->illegal = true; /* 80387 instruction. */ break;
    case 0x1C: instr->append ("FRNDINT"); break;
    case 0x1D: instr->append ("FSCALE"); break;
    case 0x1E: instr->append ("FSIN"); instr->illegal = true; /* 80387 instruction. */ break;
    case 0x1F: instr->append ("FCOS"); instr->illegal = true; /* 80387 instruction. */ break;
    default: instr->append ("???"); instr->illegal = true;
    }

  }
}

static void printModrm (instruction_t* instr) {
  ___BTPUSH;

  printModrm (instr, (instr->bytes[0] & 2) != 0, instr->bytes[0] & 1);

  ___BTPOP;
}

static void printModrm (instruction_t* instr, const bool dstRrr, const bool wide) {
  ___BTPUSH;

  const int rrr = instr->bytes[1] >> 3 & 7;

  if (dstRrr) {
    instr->append ("%s,", getReg (rrr, wide));
  }

  printModrmPointer (instr, wide);

  if (!dstRrr) {
    instr->append (",%s", getReg (rrr, wide));
  }

  ___BTPOP;
}

static void printModrmPointer (instruction_t* instr, const bool wide) {
  ___BTPUSH;

  const int mmm = instr->bytes[1] & 7;

  switch (instr->bytes[1] >> 6) {
  case 0:
    if (mmm != 6) {
      instr->append ("%s]", getModrmPointerPart (mmm));
    } else {
      instr->append ("[%04X]", readWord (instr->bytes, 2));
    }
    break;

  case 1:
    instr->append (getModrmPointerPart (mmm));
    instr->appendSignedByte (instr->bytes[2]);
    instr->append (']');
    break;

  case 2:
    instr->append ("%s+%04X]", getModrmPointerPart (mmm), readWord (instr->bytes, 2));
    break;

  case 3:
    instr->append (getReg (mmm, wide));
    break;
  }

  ___BTPOP;
}

static void printMovAccMem (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 3;
  if (instr->size <= maxInstrLength) {
    instr->append ("MOV     %s,[%04X]", getReg (0, instr->bytes[0] & 1), readWord (instr->bytes, 1));
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printMovMemAcc (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 3;
  if (instr->size <= maxInstrLength) {
    instr->append ("MOV     [%04X],%s", readWord (instr->bytes, 1), getReg (0, instr->bytes[0] & 1));
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printMovRegImm (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  bool wide = (instr->bytes[0] & 8) != 0;
  instr->size = 2 + wide;
  if (instr->size <= maxInstrLength) {
    instr->append ("MOV     %s,", getReg (instr->bytes[0] & 7, wide));
    if (wide) {
      instr->append ("%04X", readWord (instr->bytes, 1));
    } else {
      instr->append ("%02X", instr->bytes[1]);
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printMovRmImm (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  const bool wide = instr->bytes[0] & 1;
  const int dispLen = modrmExtraLength (instr->bytes[1]);
  instr->size = 3 + dispLen + wide;
  if (instr->size <= maxInstrLength) {
    instr->illegal = (instr->bytes[1] & 0x38) != 0;
    if ((instr->bytes[0] & 0x10) == 0) {
      instr->append ("MOV     ");
    } else {
      instr->append ("TEST    ");
    }
    printPointerType (instr);
    printModrmPointer (instr, wide);
    if (wide) {
      instr->append (",%04X", readWord (instr->bytes, 2 + dispLen));
    } else {
      instr->append (",%02X", instr->bytes[2 + dispLen]);
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printMovRmReg (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    instr->append ("MOV     ");
    printModrm (instr);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printMovRmSeg (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    instr->illegal = (instr->bytes[1] & 0x20) != 0;
    instr->append ("MOV     ");

    const bool dstRrr = (instr->bytes[0] & 2) != 0;
    if (dstRrr) {
      instr->append ("%s,", getSeg (instr->bytes[1]));
    }
    printModrmPointer (instr, true);
    if (!dstRrr) {
      instr->append (",%s", getSeg (instr->bytes[1]));
    }

  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printNotRm (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  const bool wide = instr->bytes[0] & 1;
  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    instr->illegal = (instr->bytes[1] & 0x38) == 8;
    instr->append (getNotGroupInstructionName (instr->bytes[1]));
    printPointerType (instr);
    printModrmPointer (instr, wide);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printPointerType (instruction_t* instr) {
  ___BTPUSH;

  if (instr->bytes[1] >> 6 != 3) {
    if (instr->bytes[0] & 1) {
      instr->append ("WORD PTR ");
    } else {
      instr->append ("BYTE PTR ");
    }
  }

  ___BTPOP;
}

static void printPopRm (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    instr->illegal = (instr->bytes[1] & 0x38) != 0;
    instr->append ("POP     ");
    printModrmPointer (instr, true);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printPushReg (instruction_t* instr) {
  ___BTPUSH;

  instr->size = 1;
  switch (instr->bytes[0] >> 3 & 3) {
  case 0: instr->append ("INC     "); break;
  case 1: instr->append ("DEC     "); break;
  case 2: instr->append ("PUSH    "); break;
  case 3: instr->append ("POP     "); break;
  }
  instr->append ("%s", getReg (instr->bytes[0] & 7, true));

  ___BTPOP;
}

static void printPushSeg (instruction_t* instr) {
  ___BTPUSH;

  instr->size = 1;
  if ((instr->bytes[0] & 0x20) == 0) {
    if (instr->bytes[0] & 1) {
      instr->append ("POP     ");
    } else {
      instr->append ("PUSH    ");
    }
    instr->append ("%s", getSeg (instr->bytes[0])); /* Note that `POP CS' is not documented by Intel. */
  } else if (instr->bytes[0] & 1) {
    switch (instr->bytes[0] >> 3) {
    case 4: instr->append ("DAA"); break;
    case 5: instr->append ("DAS"); break;
    case 6: instr->append ("AAA"); break;
    case 7: instr->append ("AAS");
    }
  } else {
    instr->append ("%s:", getSeg (instr->bytes[0]));
    instr->prefix = true;
  }

  ___BTPOP;
}

static void printRet (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 1 + (((instr->bytes[0] & 1) == 0) << 1);
  if (instr->size <= maxInstrLength) {
    switch (instr->bytes[0] & 9) {
    case 0: instr->append ("RET     %04X", readWord (instr->bytes, 1)); break;
    case 1: instr->append ("RET"); break;
    case 8: instr->append ("RETF    %04X", readWord (instr->bytes, 1)); break;
    case 9: instr->append ("RETF");
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printShift (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    instr->append (getShiftInstructionName (instr->bytes[1]));
    printPointerType (instr);
    printModrmPointer (instr, instr->bytes[0] & 1);
    if ((instr->bytes[0] & 2) != 0) {
      instr->append (",CL");
    } else {
      instr->append (",1");
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printShiftImm (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  /* 80186 instruction. */
  instr->illegal = true;
  const int dispLen = modrmExtraLength (instr->bytes[1]);
  instr->size = 3 + dispLen;
  if (instr->size <= maxInstrLength) {
    instr->append (getShiftInstructionName (instr->bytes[1]));
    printPointerType (instr);
    printModrmPointer (instr, instr->bytes[0] & 1);
    instr->append (",%02X", instr->bytes[2 + dispLen]);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

/* Total should not be larger than 63. */
static void printSpaces (int total) {
  ___BTPUSH;

  char buf[64];
  memset (buf, ' ', total);
  buf[total] = 0;
  printf ("%s", buf);

  ___BTPOP;
}

static void printStringInstruction (instruction_t* instr) {
  ___BTPUSH;

  instr->size = 1;
  switch (instr->bytes[0] >> 1 & 7) {
  case 2: instr->append ("MOVS"); break;
  case 3: instr->append ("CMPS"); break;
  case 5: instr->append ("STOS"); break;
  case 6: instr->append ("LODS"); break;
  case 7: instr->append ("SCAS");
  }
  if (instr->bytes[0] & 1) {
    instr->append ('W');
  } else {
    instr->append ('B');
  }

  ___BTPOP;
}

static void printTestAccImm (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  bool wide = (instr->bytes[0] & 1) != 0;
  instr->size = 2 + wide;
  if (instr->size <= maxInstrLength) {
    instr->append ("TEST    ");
    if (wide) {
      instr->append ("AX,%04X", readWord (instr->bytes, 1));
    } else {
      instr->append ("AL,%02X", instr->bytes[1]);
    }
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printTestRmReg (instruction_t* instr, int maxInstrLength) {
  ___BTPUSH;

  instr->size = 2 + modrmExtraLength (instr->bytes[1]);
  if (instr->size <= maxInstrLength) {
    if ((instr->bytes[0] & 2) == 0) {
      instr->append ("TEST    ");
    } else {
      instr->append ("XCHG    ");
    }
    printModrm (instr);
  } else {
    printDB (instr);
  }

  ___BTPOP;
}

static void printXchgAxReg (instruction_t* instr) {
  ___BTPUSH;

  instr->size = 1;
  int reg = instr->bytes[0] & 7;
  if (reg > 0) {
    instr->append ("XCHG    AX,%s", getReg (reg, true));
  } else {
    instr->append ("NOP");
  }

  ___BTPOP;
}

static int readWord (const unsigned char* buf, int offset) {
  ___BTPUSH;

  int result = buf[offset + 1];
  result <<= 8;
  result |= buf[offset];

  ___BTPOP;
  return result;
}
