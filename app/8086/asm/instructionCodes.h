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

#ifndef ___DEBUG_ASM_INSTRUCTION_CODES_INCLUDED
#define ___DEBUG_ASM_INSTRUCTION_CODES_INCLUDED

#define GRP_AAM    1
#define GRP_ADD    2
#define GRP_CALL   3
#define GRP_CONJMP 4
#define GRP_IN     5
#define GRP_INC    6
#define GRP_INT    7
#define GRP_JMP    8
#define GRP_LEA    9
#define GRP_MISC   10
#define GRP_MOV    11
#define GRP_NOT    12
#define GRP_OUT    13
#define GRP_PUSH   14
#define GRP_RET    15
#define GRP_SHIFT  16
#define GRP_TEST   17
#define GRP_XCHG   18

#define OPC_ADD    0
#define OPC_OR     1
#define OPC_ADC    2
#define OPC_SBB    3
#define OPC_AND    4
#define OPC_SUB    5
#define OPC_XOR    6
#define OPC_CMP    7

#define OPC_ROL    0
#define OPC_ROR    1
#define OPC_RCL    2
#define OPC_RCR    3
#define OPC_SHL    4
#define OPC_SHR    5
#define OPC_SAL    6
#define OPC_SAR    7

#define OPC_NOT    2
#define OPC_NEG    3
#define OPC_MUL    4
#define OPC_IMUL   5
#define OPC_DIV    6
#define OPC_IDIV   7

#define OPC_INC    0
#define OPC_DEC    1

#define OPC_PUSH   0
#define OPC_POP    1

#define OPC_CALL   2
#define OPC_CALLF  3

#define OPC_JMP    4
#define OPC_JMPF   5

#define OPC_IN     0
#define OPC_INT    0
#define OPC_MOV    0
#define OPC_OUT    0
#define OPC_TEST   0
#define OPC_XCHG   0

#define OPC_RET    0xC2
#define OPC_RETF   0xCA

#define OPC_AAM    0xD4
#define OPC_AAD    0xD5

#define OPC_LEA    0x8D
#define OPC_LES    0xC4
#define OPC_LDS    0xC5

#define OPC_JO     0x70
#define OPC_JNO    0x71
#define OPC_JB     0x72
#define OPC_JC     OPC_JB
#define OPC_JNAE   OPC_JB
#define OPC_JNB    0x73
#define OPC_JNC    OPC_JNB
#define OPC_JAE    OPC_JNB
#define OPC_JZ     0x74
#define OPC_JE     OPC_JZ
#define OPC_JNZ    0x75
#define OPC_JNE    OPC_JNZ
#define OPC_JBE    0x76
#define OPC_JNA    OPC_JBE
#define OPC_JA     0x77
#define OPC_JNBE   OPC_JA
#define OPC_JS     0x78
#define OPC_JNS    0x79
#define OPC_JPE    0x7A
#define OPC_JP     OPC_JPE
#define OPC_JPO    0x7B
#define OPC_JNP    OPC_JPO
#define OPC_JL     0x7C
#define OPC_JNGE   OPC_JL
#define OPC_JGE    0x7D
#define OPC_JNL    OPC_JGE
#define OPC_JLE    0x7E
#define OPC_JNG    OPC_JLE
#define OPC_JG     0x7F
#define OPC_JNLE   OPC_JG
#define OPC_LOOPNZ 0xE0
#define OPC_LOOPNE OPC_LOOPNZ
#define OPC_LOOPZ  0xE1
#define OPC_LOOPE  OPC_LOOPZ
#define OPC_LOOP   0xE2
#define OPC_JCXZ   0xE3

#define OPC_ES     0x26
#define OPC_DAA    0x27
#define OPC_CS     0x2E
#define OPC_DAS    0x2F
#define OPC_SS     0x36
#define OPC_AAA    0x37
#define OPC_DS     0x3E
#define OPC_AAS    0x3F
#define OPC_NOP    0x90
#define OPC_CBW    0x98
#define OPC_CWD    0x99
#define OPC_WAIT   0x9B
#define OPC_PUSHF  0x9C
#define OPC_POPF   0x9D
#define OPC_SAHF   0x9E
#define OPC_LAHF   0x9F
#define OPC_MOVSB  0xA4
#define OPC_MOVSW  0xA5
#define OPC_CMPSB  0xA6
#define OPC_CMPSW  0xA7
#define OPC_STOSB  0xAA
#define OPC_STOSW  0xAB
#define OPC_LODSB  0xAC
#define OPC_LODSW  0xAD
#define OPC_SCASB  0xAE
#define OPC_SCASW  0xAF
#define OPC_INTO   0xCE
#define OPC_IRET   0xCF
#define OPC_XLAT   0xD7
#define OPC_LOCK   0xF0
#define OPC_REPNZ  0xF2
#define OPC_REPNE  OPC_REPNZ
#define OPC_REPZ   0xF3
#define OPC_REPE   OPC_REPZ
#define OPC_REP    OPC_REPZ
#define OPC_HLT    0xF4
#define OPC_CMC    0xF5
#define OPC_CLC    0xF8
#define OPC_STC    0xF9
#define OPC_CLI    0xFA
#define OPC_STI    0xFB
#define OPC_CLD    0xFC
#define OPC_STD    0xFD

#endif
