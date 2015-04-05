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

#ifndef ___GVASM__INSTRUCTION_CODES_INCLUDED
#define ___GVASM__INSTRUCTION_CODES_INCLUDED

#define WIDTH_IID_GRP 5

#define MASK_IID_GRP ((1 << WIDTH_IID_GRP) - 1)

#define GRP_AAM    1
#define GRP_ADD    2
#define GRP_CALL   3
#define GRP_CONJMP 4
#define GRP_DATA   5
#define GRP_IN     6
#define GRP_INC    7
#define GRP_INT    8
#define GRP_JMP    9
#define GRP_LEA    10
#define GRP_MISC   11
#define GRP_MOV    12
#define GRP_NOT    13
#define GRP_OUT    14
#define GRP_PUSH   15
#define GRP_RET    16
#define GRP_SHIFT  17
#define GRP_TEST   18
#define GRP_XCHG   19

#define IID_ADD    (0 << WIDTH_IID_GRP | GRP_ADD)
#define IID_OR     (1 << WIDTH_IID_GRP | GRP_ADD)
#define IID_ADC    (2 << WIDTH_IID_GRP | GRP_ADD)
#define IID_SBB    (3 << WIDTH_IID_GRP | GRP_ADD)
#define IID_AND    (4 << WIDTH_IID_GRP | GRP_ADD)
#define IID_SUB    (5 << WIDTH_IID_GRP | GRP_ADD)
#define IID_XOR    (6 << WIDTH_IID_GRP | GRP_ADD)
#define IID_CMP    (7 << WIDTH_IID_GRP | GRP_ADD)

#define IID_ROL    (0 << WIDTH_IID_GRP | GRP_SHIFT)
#define IID_ROR    (1 << WIDTH_IID_GRP | GRP_SHIFT)
#define IID_RCL    (2 << WIDTH_IID_GRP | GRP_SHIFT)
#define IID_RCR    (3 << WIDTH_IID_GRP | GRP_SHIFT)
#define IID_SHL    (4 << WIDTH_IID_GRP | GRP_SHIFT)
#define IID_SHR    (5 << WIDTH_IID_GRP | GRP_SHIFT)
#define IID_SAL    (6 << WIDTH_IID_GRP | GRP_SHIFT)
#define IID_SAR    (7 << WIDTH_IID_GRP | GRP_SHIFT)

#define IID_NOT    (2 << WIDTH_IID_GRP | GRP_NOT)
#define IID_NEG    (3 << WIDTH_IID_GRP | GRP_NOT)
#define IID_MUL    (4 << WIDTH_IID_GRP | GRP_NOT)
#define IID_IMUL   (5 << WIDTH_IID_GRP | GRP_NOT)
#define IID_DIV    (6 << WIDTH_IID_GRP | GRP_NOT)
#define IID_IDIV   (7 << WIDTH_IID_GRP | GRP_NOT)

#define IID_INC    (0 << WIDTH_IID_GRP | GRP_INC)
#define IID_DEC    (1 << WIDTH_IID_GRP | GRP_INC)

#define IID_PUSH   (0 << WIDTH_IID_GRP | GRP_PUSH)
#define IID_POP    (1 << WIDTH_IID_GRP | GRP_PUSH)

#define IID_CALL   (2 << WIDTH_IID_GRP | GRP_CALL)
#define IID_CALLF  (3 << WIDTH_IID_GRP | GRP_CALL)

#define IID_JMP    (4 << WIDTH_IID_GRP | GRP_JMP)
#define IID_JMPF   (5 << WIDTH_IID_GRP | GRP_JMP)

#define IID_IN     (0 << WIDTH_IID_GRP | GRP_IN)
#define IID_INT    (0 << WIDTH_IID_GRP | GRP_INT)
#define IID_MOV    (0 << WIDTH_IID_GRP | GRP_MOV)
#define IID_OUT    (0 << WIDTH_IID_GRP | GRP_OUT)
#define IID_TEST   (0 << WIDTH_IID_GRP | GRP_TEST)
#define IID_XCHG   (0 << WIDTH_IID_GRP | GRP_XCHG)

#define IID_RET    (0xC2 << WIDTH_IID_GRP | GRP_RET)
#define IID_RETF   (0xCA << WIDTH_IID_GRP | GRP_RET)

#define IID_AAM    (0xD4 << WIDTH_IID_GRP | GRP_AAM)
#define IID_AAD    (0xD5 << WIDTH_IID_GRP | GRP_AAM)

#define IID_LEA    (0x8D << WIDTH_IID_GRP | GRP_LEA)
#define IID_LES    (0xC4 << WIDTH_IID_GRP | GRP_LEA)
#define IID_LDS    (0xC5 << WIDTH_IID_GRP | GRP_LEA)

#define IID_JO     (0x70 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNO    (0x71 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JB     (0x72 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JC     IID_JB
#define IID_JNAE   IID_JB
#define IID_JNB    (0x73 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNC    IID_JNB
#define IID_JAE    IID_JNB
#define IID_JZ     (0x74 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JE     IID_JZ
#define IID_JNZ    (0x75 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNE    IID_JNZ
#define IID_JBE    (0x76 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNA    IID_JBE
#define IID_JA     (0x77 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNBE   IID_JA
#define IID_JS     (0x78 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNS    (0x79 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JPE    (0x7A << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JP     IID_JPE
#define IID_JPO    (0x7B << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNP    IID_JPO
#define IID_JL     (0x7C << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNGE   IID_JL
#define IID_JGE    (0x7D << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNL    IID_JGE
#define IID_JLE    (0x7E << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNG    IID_JLE
#define IID_JG     (0x7F << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JNLE   IID_JG
#define IID_LOOPNZ (0xE0 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_LOOPNE IID_LOOPNZ
#define IID_LOOPZ  (0xE1 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_LOOPE  IID_LOOPZ
#define IID_LOOP   (0xE2 << WIDTH_IID_GRP | GRP_CONJMP)
#define IID_JCXZ   (0xE3 << WIDTH_IID_GRP | GRP_CONJMP)

#define IID_ES_O   (0x26 << WIDTH_IID_GRP | GRP_MISC)
#define IID_DAA    (0x27 << WIDTH_IID_GRP | GRP_MISC)
#define IID_CS_O   (0x2E << WIDTH_IID_GRP | GRP_MISC)
#define IID_DAS    (0x2F << WIDTH_IID_GRP | GRP_MISC)
#define IID_SS_O   (0x36 << WIDTH_IID_GRP | GRP_MISC)
#define IID_AAA    (0x37 << WIDTH_IID_GRP | GRP_MISC)
#define IID_DS_O   (0x3E << WIDTH_IID_GRP | GRP_MISC)
#define IID_AAS    (0x3F << WIDTH_IID_GRP | GRP_MISC)
#define IID_NOP    (0x90 << WIDTH_IID_GRP | GRP_MISC)
#define IID_CBW    (0x98 << WIDTH_IID_GRP | GRP_MISC)
#define IID_CWD    (0x99 << WIDTH_IID_GRP | GRP_MISC)
#define IID_WAIT   (0x9B << WIDTH_IID_GRP | GRP_MISC)
#define IID_PUSHF  (0x9C << WIDTH_IID_GRP | GRP_MISC)
#define IID_POPF   (0x9D << WIDTH_IID_GRP | GRP_MISC)
#define IID_SAHF   (0x9E << WIDTH_IID_GRP | GRP_MISC)
#define IID_LAHF   (0x9F << WIDTH_IID_GRP | GRP_MISC)
#define IID_MOVSB  (0xA4 << WIDTH_IID_GRP | GRP_MISC)
#define IID_MOVSW  (0xA5 << WIDTH_IID_GRP | GRP_MISC)
#define IID_CMPSB  (0xA6 << WIDTH_IID_GRP | GRP_MISC)
#define IID_CMPSW  (0xA7 << WIDTH_IID_GRP | GRP_MISC)
#define IID_STOSB  (0xAA << WIDTH_IID_GRP | GRP_MISC)
#define IID_STOSW  (0xAB << WIDTH_IID_GRP | GRP_MISC)
#define IID_LODSB  (0xAC << WIDTH_IID_GRP | GRP_MISC)
#define IID_LODSW  (0xAD << WIDTH_IID_GRP | GRP_MISC)
#define IID_SCASB  (0xAE << WIDTH_IID_GRP | GRP_MISC)
#define IID_SCASW  (0xAF << WIDTH_IID_GRP | GRP_MISC)
#define IID_INTO   (0xCE << WIDTH_IID_GRP | GRP_MISC)
#define IID_IRET   (0xCF << WIDTH_IID_GRP | GRP_MISC)
#define IID_XLAT   (0xD7 << WIDTH_IID_GRP | GRP_MISC)
#define IID_LOCK   (0xF0 << WIDTH_IID_GRP | GRP_MISC)
#define IID_REPNZ  (0xF2 << WIDTH_IID_GRP | GRP_MISC)
#define IID_REPNE  IID_REPNZ
#define IID_REPZ   (0xF3 << WIDTH_IID_GRP | GRP_MISC)
#define IID_REPE   IID_REPZ
#define IID_REP    IID_REPZ
#define IID_HLT    (0xF4 << WIDTH_IID_GRP | GRP_MISC)
#define IID_CMC    (0xF5 << WIDTH_IID_GRP | GRP_MISC)
#define IID_CLC    (0xF8 << WIDTH_IID_GRP | GRP_MISC)
#define IID_STC    (0xF9 << WIDTH_IID_GRP | GRP_MISC)
#define IID_CLI    (0xFA << WIDTH_IID_GRP | GRP_MISC)
#define IID_STI    (0xFB << WIDTH_IID_GRP | GRP_MISC)
#define IID_CLD    (0xFC << WIDTH_IID_GRP | GRP_MISC)
#define IID_STD    (0xFD << WIDTH_IID_GRP | GRP_MISC)

#define IID_DB     (0 << WIDTH_IID_GRP | GRP_DATA)
#define IID_DC     (1 << WIDTH_IID_GRP | GRP_DATA)
#define IID_DW     (2 << WIDTH_IID_GRP | GRP_DATA)

#endif
