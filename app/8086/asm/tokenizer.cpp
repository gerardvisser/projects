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

#include <ctype.h>
#include <string.h>
#include "tokenizer.h"
#include "tokenTypes.h"
#include "instructionCodes.h"
#include <lombok/base/errors.h>
#include <lombok/base/uchar.h>
#include <lombok/util/arrayList.h>

REFL_IMPL (tokenizer_t, object_t)

static int identifyInstruction (const char* input, int* inputIndex);
static int identifyNLetterString (const char* str, const char* words, int n);

tokenizer_t::tokenizer_t (void) {
  ___CBTPUSH;
  ___CBTPOP;
}

tokenizer_t::~tokenizer_t (void) {
  ___CBTPUSH;
  ___CBTPOP;
}

/* Restrictions on input:
   1. First character is non-whitespace.
   2. Every whitespace is followed by and preceded by a non-whitespace character.
   3. No whitespace precedes the terminating null-character.
   4. The only whitespace character occurring is 0x20 (' ').   */
list_t* tokenizer_t::getTokens (error_t* error, const char* input) {
  ___CBTPUSH;

  reset ();
  arrayList_t* tokens = new arrayList_t ();
  int instrId = identifyInstruction (input, &m_inputIndex);
  if (instrId > 0) {
    tokens->append (new token_t (TTP_INSTRUCTION, instrId, 0));
    while (input[m_inputIndex] != 0 && m_errorCode == 0) {
      if (input[m_inputIndex] == ' ') ++m_inputIndex;
      token_t* token = nextToken (input);
      if (token != NULL) {
        tokens->append (token);
      }
    }
    tokens->append (new token_t (TTP_LAST, 0, m_inputIndex));
  } else {
    m_errorCode = 1;
  }
  error->location = m_inputIndex;
  error->code = m_errorCode;

  ___CBTPOP;
  return tokens;
}

token_t* tokenizer_t::nextToken (const char* input) {
  ___CBTPUSH;

  token_t* token;
  if (input[m_inputIndex] > 0x40 && input[m_inputIndex] < 0x5B) {
    token = tryCreatingPointerDeterminerToken (input);
    if (token == NULL && m_errorCode == 0) {
      token = tryCreatingRegisterToken (input);
      if (token == NULL) {
        m_errorCode = input[m_inputIndex] > 0x46;
        if (m_errorCode == 0) {
          token = tryCreatingNumberToken (input);
        }
      }
    }
  } else if (input[m_inputIndex] > 0x2F && input[m_inputIndex] < 0x3A) {
    token = tryCreatingNumberToken (input);
  } else {
    token = tryCreatingCharToken (input);
  }

  ___CBTPOP;
  return token;
}

void tokenizer_t::reset (void) {
  ___CBTPUSH;

  m_errorCode = 0;
  m_inputIndex = 0;

  ___CBTPOP;
}

token_t* tokenizer_t::tryCreatingCharToken (const char* input) {
  ___CBTPUSH;

  int i = 0;
  token_t* token = NULL;
  const char* const chars = "+,-:[]";
  while (!(chars[i] == 0 || chars[i] == input[m_inputIndex])) {
    ++i;
  }
  m_errorCode = chars[i] == 0;
  if (m_errorCode == 0) {
    token = new token_t (TTP_CHAR, input[m_inputIndex], m_inputIndex);
    ++m_inputIndex;
  }

  ___CBTPOP;
  return token;
}

token_t* tokenizer_t::tryCreatingNumberToken (const char* input) {
  ___CBTPUSH;

  token_t* token = NULL;
  int tokenPosition = m_inputIndex;
  while (input[m_inputIndex] == '0') {
    ++m_inputIndex;
  }
  int value = 0;
  int digitsRead = 0;
  while (isxdigit (input[m_inputIndex]) && digitsRead < 4) {
    value <<= 4;
    value |= uchar_hexDigitValue (input[m_inputIndex]);
    ++m_inputIndex;
    ++digitsRead;
  }
  m_errorCode = isxdigit (input[m_inputIndex]);
  if (m_errorCode == 0) {
    token = new token_t (TTP_NUMBER, value, tokenPosition);
  }

  ___CBTPOP;
  return token;
}

token_t* tokenizer_t::tryCreatingPointerDeterminerToken (const char* input) {
  ___CBTPUSH;

  token_t* token = NULL;
  const char* const words = "BYTEWORD";
  int i = identifyNLetterString (input + m_inputIndex, words, 4);
  if (i > -1) {
    int tokenPosition = m_inputIndex;
    m_inputIndex += 4;
    m_inputIndex += input[m_inputIndex] == ' ';
    if (strncmp (input + m_inputIndex, "PTR", 3) == 0) {
      token = new token_t (TTP_PTRDETERMINER, i, tokenPosition);
      m_inputIndex += 3;
    } else {
      m_errorCode = 1;
    }
  }

  ___CBTPOP;
  return token;
}

token_t* tokenizer_t::tryCreatingRegisterToken (const char* input) {
  ___CBTPUSH;

  token_t* token = NULL;
  const char* const words = "ALCLDLBLAHCHDHBHAXCXDXBXSPBPSIDIESCSSSDS";
  int i = identifyNLetterString (input + m_inputIndex, words, 2);
  if (i > -1) {
    if (i > 15) {
      token = new token_t (TTP_SEGREGISTER, i - 16, m_inputIndex);
    } else {
      token = new token_t (TTP_REGISTER, i, m_inputIndex);
    }
    m_inputIndex += 2;
  }

  ___CBTPOP;
  return token;
}

#define INSTR_AAA    (GRP_MISC << 8 | OPC_AAA)
#define INSTR_AAD    (GRP_AAM << 8 | OPC_AAD)
#define INSTR_AAM    (GRP_AAM << 8 | OPC_AAM)
#define INSTR_AAS    (GRP_MISC << 8 | OPC_AAS)
#define INSTR_ADC    (GRP_ADD << 8 | OPC_ADC)
#define INSTR_ADD    (GRP_ADD << 8 | OPC_ADD)
#define INSTR_AND    (GRP_ADD << 8 | OPC_AND)
#define INSTR_CALL   (GRP_CALL << 8 | OPC_CALL)
#define INSTR_CALLF  (GRP_CALL << 8 | OPC_CALLF)
#define INSTR_CBW    (GRP_MISC << 8 | OPC_CBW)
#define INSTR_CLC    (GRP_MISC << 8 | OPC_CLC)
#define INSTR_CLD    (GRP_MISC << 8 | OPC_CLD)
#define INSTR_CLI    (GRP_MISC << 8 | OPC_CLI)
#define INSTR_CMC    (GRP_MISC << 8 | OPC_CMC)
#define INSTR_CMP    (GRP_ADD << 8 | OPC_CMP)
#define INSTR_CMPSB  (GRP_MISC << 8 | OPC_CMPSB)
#define INSTR_CMPSW  (GRP_MISC << 8 | OPC_CMPSW)
#define INSTR_CS     (GRP_MISC << 8 | OPC_CS)
#define INSTR_CWD    (GRP_MISC << 8 | OPC_CWD)
#define INSTR_DAA    (GRP_MISC << 8 | OPC_DAA)
#define INSTR_DAS    (GRP_MISC << 8 | OPC_DAS)
#define INSTR_DEC    (GRP_INC << 8 | OPC_DEC)
#define INSTR_DIV    (GRP_NOT << 8 | OPC_DIV)
#define INSTR_DS     (GRP_MISC << 8 | OPC_DS)
#define INSTR_ES     (GRP_MISC << 8 | OPC_ES)
#define INSTR_HLT    (GRP_MISC << 8 | OPC_HLT)
#define INSTR_IDIV   (GRP_NOT << 8 | OPC_IDIV)
#define INSTR_IMUL   (GRP_NOT << 8 | OPC_IMUL)
#define INSTR_IN     (GRP_IN << 8 | OPC_IN)
#define INSTR_INC    (GRP_INC << 8 | OPC_INC)
#define INSTR_INT    (GRP_INT << 8 | OPC_INT)
#define INSTR_INTO   (GRP_MISC << 8 | OPC_INTO)
#define INSTR_IRET   (GRP_MISC << 8 | OPC_IRET)
#define INSTR_JA     (GRP_CONJMP << 8 | OPC_JA)
#define INSTR_JAE    (GRP_CONJMP << 8 | OPC_JAE)
#define INSTR_JB     (GRP_CONJMP << 8 | OPC_JB)
#define INSTR_JBE    (GRP_CONJMP << 8 | OPC_JBE)
#define INSTR_JC     (GRP_CONJMP << 8 | OPC_JC)
#define INSTR_JCXZ   (GRP_CONJMP << 8 | OPC_JCXZ)
#define INSTR_JE     (GRP_CONJMP << 8 | OPC_JE)
#define INSTR_JG     (GRP_CONJMP << 8 | OPC_JG)
#define INSTR_JGE    (GRP_CONJMP << 8 | OPC_JGE)
#define INSTR_JL     (GRP_CONJMP << 8 | OPC_JL)
#define INSTR_JLE    (GRP_CONJMP << 8 | OPC_JLE)
#define INSTR_JMP    (GRP_JMP << 8 | OPC_JMP)
#define INSTR_JMPF   (GRP_JMP << 8 | OPC_JMPF)
#define INSTR_JNA    (GRP_CONJMP << 8 | OPC_JNA)
#define INSTR_JNAE   (GRP_CONJMP << 8 | OPC_JNAE)
#define INSTR_JNB    (GRP_CONJMP << 8 | OPC_JNB)
#define INSTR_JNBE   (GRP_CONJMP << 8 | OPC_JNBE)
#define INSTR_JNC    (GRP_CONJMP << 8 | OPC_JNC)
#define INSTR_JNE    (GRP_CONJMP << 8 | OPC_JNE)
#define INSTR_JNG    (GRP_CONJMP << 8 | OPC_JNG)
#define INSTR_JNGE   (GRP_CONJMP << 8 | OPC_JNGE)
#define INSTR_JNL    (GRP_CONJMP << 8 | OPC_JNL)
#define INSTR_JNLE   (GRP_CONJMP << 8 | OPC_JNLE)
#define INSTR_JNO    (GRP_CONJMP << 8 | OPC_JNO)
#define INSTR_JNP    (GRP_CONJMP << 8 | OPC_JNP)
#define INSTR_JNS    (GRP_CONJMP << 8 | OPC_JNS)
#define INSTR_JNZ    (GRP_CONJMP << 8 | OPC_JNZ)
#define INSTR_JO     (GRP_CONJMP << 8 | OPC_JO)
#define INSTR_JP     (GRP_CONJMP << 8 | OPC_JP)
#define INSTR_JPE    (GRP_CONJMP << 8 | OPC_JPE)
#define INSTR_JPO    (GRP_CONJMP << 8 | OPC_JPO)
#define INSTR_JS     (GRP_CONJMP << 8 | OPC_JS)
#define INSTR_JZ     (GRP_CONJMP << 8 | OPC_JZ)
#define INSTR_LAHF   (GRP_MISC << 8 | OPC_LAHF)
#define INSTR_LDS    (GRP_LEA << 8 | OPC_LDS)
#define INSTR_LEA    (GRP_LEA << 8 | OPC_LEA)
#define INSTR_LES    (GRP_LEA << 8 | OPC_LES)
#define INSTR_LOCK   (GRP_MISC << 8 | OPC_LOCK)
#define INSTR_LODSB  (GRP_MISC << 8 | OPC_LODSB)
#define INSTR_LODSW  (GRP_MISC << 8 | OPC_LODSW)
#define INSTR_LOOP   (GRP_CONJMP << 8 | OPC_LOOP)
#define INSTR_LOOPE  (GRP_CONJMP << 8 | OPC_LOOPE)
#define INSTR_LOOPNE (GRP_CONJMP << 8 | OPC_LOOPNE)
#define INSTR_LOOPNZ (GRP_CONJMP << 8 | OPC_LOOPNZ)
#define INSTR_LOOPZ  (GRP_CONJMP << 8 | OPC_LOOPZ)
#define INSTR_MOV    (GRP_MOV << 8 | OPC_MOV)
#define INSTR_MOVSB  (GRP_MISC << 8 | OPC_MOVSB)
#define INSTR_MOVSW  (GRP_MISC << 8 | OPC_MOVSW)
#define INSTR_MUL    (GRP_NOT << 8 | OPC_MUL)
#define INSTR_NEG    (GRP_NOT << 8 | OPC_NEG)
#define INSTR_NOP    (GRP_MISC << 8 | OPC_NOP)
#define INSTR_NOT    (GRP_NOT << 8 | OPC_NOT)
#define INSTR_OR     (GRP_ADD << 8 | OPC_OR)
#define INSTR_OUT    (GRP_OUT << 8 | OPC_OUT)
#define INSTR_POP    (GRP_PUSH << 8 | OPC_POP)
#define INSTR_POPF   (GRP_MISC << 8 | OPC_POPF)
#define INSTR_PUSH   (GRP_PUSH << 8 | OPC_PUSH)
#define INSTR_PUSHF  (GRP_MISC << 8 | OPC_PUSHF)
#define INSTR_RCL    (GRP_SHIFT << 8 | OPC_RCL)
#define INSTR_RCR    (GRP_SHIFT << 8 | OPC_RCR)
#define INSTR_REP    (GRP_MISC << 8 | OPC_REP)
#define INSTR_REPE   (GRP_MISC << 8 | OPC_REPE)
#define INSTR_REPNE  (GRP_MISC << 8 | OPC_REPNE)
#define INSTR_REPNZ  (GRP_MISC << 8 | OPC_REPNZ)
#define INSTR_REPZ   (GRP_MISC << 8 | OPC_REPZ)
#define INSTR_RET    (GRP_RET << 8 | OPC_RET)
#define INSTR_RETF   (GRP_RET << 8 | OPC_RETF)
#define INSTR_ROL    (GRP_SHIFT << 8 | OPC_ROL)
#define INSTR_ROR    (GRP_SHIFT << 8 | OPC_ROR)
#define INSTR_SAHF   (GRP_MISC << 8 | OPC_SAHF)
#define INSTR_SAL    (GRP_SHIFT << 8 | OPC_SAL)
#define INSTR_SAR    (GRP_SHIFT << 8 | OPC_SAR)
#define INSTR_SBB    (GRP_ADD << 8 | OPC_SBB)
#define INSTR_SCASB  (GRP_MISC << 8 | OPC_SCASB)
#define INSTR_SCASW  (GRP_MISC << 8 | OPC_SCASW)
#define INSTR_SHL    (GRP_SHIFT << 8 | OPC_SHL)
#define INSTR_SHR    (GRP_SHIFT << 8 | OPC_SHR)
#define INSTR_SS     (GRP_MISC << 8 | OPC_SS)
#define INSTR_STC    (GRP_MISC << 8 | OPC_STC)
#define INSTR_STD    (GRP_MISC << 8 | OPC_STD)
#define INSTR_STI    (GRP_MISC << 8 | OPC_STI)
#define INSTR_STOSB  (GRP_MISC << 8 | OPC_STOSB)
#define INSTR_STOSW  (GRP_MISC << 8 | OPC_STOSW)
#define INSTR_SUB    (GRP_ADD << 8 | OPC_SUB)
#define INSTR_TEST   (GRP_TEST << 8 | OPC_TEST)
#define INSTR_WAIT   (GRP_MISC << 8 | OPC_WAIT)
#define INSTR_XCHG   (GRP_XCHG << 8 | OPC_XCHG)
#define INSTR_XLAT   (GRP_MISC << 8 | OPC_XLAT)
#define INSTR_XOR    (GRP_ADD << 8 | OPC_XOR)

static int identifyNLetterString (const char* str, const char* words, int n) {
  ___BTPUSH;

  int i = 0;
  while (!(words[i] == 0 || strncmp (str, words + i, n) == 0)) {
    i += n;
  }

  ___BTPOP;
  return words[i] != 0 ? i / n : -1;
}

static int identify2LetterInstruction (const char* input, int* inputIndex) {
  ___BTPUSH;
  const char* const words = "DBINJAJBJCJEJGJLJOJPJSJZOR";
  const int n = 2;
  int instrId;

  switch (identifyNLetterString (input + inputIndex[0], words, n)) {
  case 0: instrId = -1; break;
  case 1: instrId = INSTR_IN; break;
  case 2: instrId = INSTR_JA; break;
  case 3: instrId = INSTR_JB; break;
  case 4: instrId = INSTR_JC; break;
  case 5: instrId = INSTR_JE; break;
  case 6: instrId = INSTR_JG; break;
  case 7: instrId = INSTR_JL; break;
  case 8: instrId = INSTR_JO; break;
  case 9: instrId = INSTR_JP; break;
  case 10: instrId = INSTR_JS; break;
  case 11: instrId = INSTR_JZ; break;
  case 12: instrId = INSTR_OR; break;
  default: instrId = 0;
  }
  if (instrId > 0) {
    inputIndex[0] += n;
  }

  ___BTPOP;
  return instrId;
}

static int identify3LetterInstruction (const char* input, int* inputIndex) {
  ___BTPUSH;
  const char* const words = "AAAAADAAMAASADCADDANDCBWCLCCLDCLICMCCMPCS:CWDDAADASDECDIVDS:ES:HLTINCINTJAEJBEJGEJLEJMPJNAJNBJNCJNEJNGJNLJNOJNPJNSJNZJPEJPOLDSLEALESMOVMULNEGNOPNOTOUTPOPRCLRCRREPRETROLRORSALSARSBBSHLSHRSS:STCSTDSTISUBXOR";
  const int n = 3;
  int instrId;

  switch (identifyNLetterString (input + inputIndex[0], words, n)) {
  case 0: instrId = INSTR_AAA; break;
  case 1: instrId = INSTR_AAD; break;
  case 2: instrId = INSTR_AAM; break;
  case 3: instrId = INSTR_AAS; break;
  case 4: instrId = INSTR_ADC; break;
  case 5: instrId = INSTR_ADD; break;
  case 6: instrId = INSTR_AND; break;
  case 7: instrId = INSTR_CBW; break;
  case 8: instrId = INSTR_CLC; break;
  case 9: instrId = INSTR_CLD; break;
  case 10: instrId = INSTR_CLI; break;
  case 11: instrId = INSTR_CMC; break;
  case 12: instrId = INSTR_CMP; break;
  case 13: instrId = INSTR_CS; break;
  case 14: instrId = INSTR_CWD; break;
  case 15: instrId = INSTR_DAA; break;
  case 16: instrId = INSTR_DAS; break;
  case 17: instrId = INSTR_DEC; break;
  case 18: instrId = INSTR_DIV; break;
  case 19: instrId = INSTR_DS; break;
  case 20: instrId = INSTR_ES; break;
  case 21: instrId = INSTR_HLT; break;
  case 22: instrId = INSTR_INC; break;
  case 23: instrId = INSTR_INT; break;
  case 24: instrId = INSTR_JAE; break;
  case 25: instrId = INSTR_JBE; break;
  case 26: instrId = INSTR_JGE; break;
  case 27: instrId = INSTR_JLE; break;
  case 28: {
    int i = input[inputIndex[0] + n] == ' ';
    if (strncmp (input + inputIndex[0] + n + i, "FAR", 3) == 0) {
      inputIndex[0] += 3 + i;
      instrId = INSTR_JMPF;
    } else {
      instrId = INSTR_JMP;
    }
  } break;
  case 29: instrId = INSTR_JNA; break;
  case 30: instrId = INSTR_JNB; break;
  case 31: instrId = INSTR_JNC; break;
  case 32: instrId = INSTR_JNE; break;
  case 33: instrId = INSTR_JNG; break;
  case 34: instrId = INSTR_JNL; break;
  case 35: instrId = INSTR_JNO; break;
  case 36: instrId = INSTR_JNP; break;
  case 37: instrId = INSTR_JNS; break;
  case 38: instrId = INSTR_JNZ; break;
  case 39: instrId = INSTR_JPE; break;
  case 40: instrId = INSTR_JPO; break;
  case 41: instrId = INSTR_LDS; break;
  case 42: instrId = INSTR_LEA; break;
  case 43: instrId = INSTR_LES; break;
  case 44: instrId = INSTR_MOV; break;
  case 45: instrId = INSTR_MUL; break;
  case 46: instrId = INSTR_NEG; break;
  case 47: instrId = INSTR_NOP; break;
  case 48: instrId = INSTR_NOT; break;
  case 49: instrId = INSTR_OUT; break;
  case 50: instrId = INSTR_POP; break;
  case 51: instrId = INSTR_RCL; break;
  case 52: instrId = INSTR_RCR; break;
  case 53: instrId = INSTR_REP; break;
  case 54: instrId = INSTR_RET; break;
  case 55: instrId = INSTR_ROL; break;
  case 56: instrId = INSTR_ROR; break;
  case 57: instrId = INSTR_SAL; break;
  case 58: instrId = INSTR_SAR; break;
  case 59: instrId = INSTR_SBB; break;
  case 60: instrId = INSTR_SHL; break;
  case 61: instrId = INSTR_SHR; break;
  case 62: instrId = INSTR_SS; break;
  case 63: instrId = INSTR_STC; break;
  case 64: instrId = INSTR_STD; break;
  case 65: instrId = INSTR_STI; break;
  case 66: instrId = INSTR_SUB; break;
  case 67: instrId = INSTR_XOR; break;
  default: instrId = 0;
  }
  if (instrId > 0) {
    inputIndex[0] += n;
  }

  ___BTPOP;
  return instrId;
}

static int identify4LetterInstruction (const char* input, int* inputIndex) {
  ___BTPUSH;
  const char* const words = "CALLIDIVIMULINTOIRETJCXZJNAEJNBEJNGEJNLELAHFLOCKLOOPPOPFPUSHREPEREPZRETFSAHFTESTWAITXCHGXLAT";
  const int n = 4;
  int instrId;

  switch (identifyNLetterString (input + inputIndex[0], words, n)) {
  case 0: {
    int i = input[inputIndex[0] + n] == ' ';
    if (strncmp (input + inputIndex[0] + n + i, "FAR", 3) == 0) {
      inputIndex[0] += 3 + i;
      instrId = INSTR_CALLF;
    } else {
      instrId = INSTR_CALL;
    }
  } break;
  case 1: instrId = INSTR_IDIV; break;
  case 2: instrId = INSTR_IMUL; break;
  case 3: instrId = INSTR_INTO; break;
  case 4: instrId = INSTR_IRET; break;
  case 5: instrId = INSTR_JCXZ; break;
  case 6: instrId = INSTR_JNAE; break;
  case 7: instrId = INSTR_JNBE; break;
  case 8: instrId = INSTR_JNGE; break;
  case 9: instrId = INSTR_JNLE; break;
  case 10: instrId = INSTR_LAHF; break;
  case 11: instrId = INSTR_LOCK; break;
  case 12: instrId = INSTR_LOOP; break;
  case 13: instrId = INSTR_POPF; break;
  case 14: instrId = INSTR_PUSH; break;
  case 15: instrId = INSTR_REPE; break;
  case 16: instrId = INSTR_REPZ; break;
  case 17: instrId = INSTR_RETF; break;
  case 18: instrId = INSTR_SAHF; break;
  case 19: instrId = INSTR_TEST; break;
  case 20: instrId = INSTR_WAIT; break;
  case 21: instrId = INSTR_XCHG; break;
  case 22: instrId = INSTR_XLAT; break;
  default: instrId = 0;
  }
  if (instrId > 0) {
    inputIndex[0] += n;
  }

  ___BTPOP;
  return instrId;
}

static int identify5LetterInstruction (const char* input, int* inputIndex) {
  ___BTPUSH;
  const char* const words = "CMPSBCMPSWLODSBLODSWLOOPELOOPZMOVSBMOVSWPUSHFREPNEREPNZSCASBSCASWSTOSBSTOSW";
  const int n = 5;
  int instrId;

  switch (identifyNLetterString (input + inputIndex[0], words, n)) {
  case 0: instrId = INSTR_CMPSB; break;
  case 1: instrId = INSTR_CMPSW; break;
  case 2: instrId = INSTR_LODSB; break;
  case 3: instrId = INSTR_LODSW; break;
  case 4: instrId = INSTR_LOOPE; break;
  case 5: instrId = INSTR_LOOPZ; break;
  case 6: instrId = INSTR_MOVSB; break;
  case 7: instrId = INSTR_MOVSW; break;
  case 8: instrId = INSTR_PUSHF; break;
  case 9: instrId = INSTR_REPNE; break;
  case 10: instrId = INSTR_REPNZ; break;
  case 11: instrId = INSTR_SCASB; break;
  case 12: instrId = INSTR_SCASW; break;
  case 13: instrId = INSTR_STOSB; break;
  case 14: instrId = INSTR_STOSW; break;
  default: instrId = 0;
  }
  if (instrId > 0) {
    inputIndex[0] += n;
  }

  ___BTPOP;
  return instrId;
}

static int identify6LetterInstruction (const char* input, int* inputIndex) {
  ___BTPUSH;
  const char* const words = "LOOPNELOOPNZ";
  const int n = 6;
  int instrId;

  switch (identifyNLetterString (input + inputIndex[0], words, n)) {
  case 0: instrId = INSTR_LOOPNE; break;
  case 1: instrId = INSTR_LOOPNZ; break;
  default: instrId = 0;
  }
  if (instrId > 0) {
    inputIndex[0] += n;
  }

  ___BTPOP;
  return instrId;
}

static int identifyInstruction (const char* input, int* inputIndex) {
  ___BTPUSH;

  int instrId = identify6LetterInstruction (input, inputIndex);
  if (instrId == 0) {
    instrId = identify5LetterInstruction (input, inputIndex);
    if (instrId == 0) {
      instrId = identify4LetterInstruction (input, inputIndex);
      if (instrId == 0) {
        instrId = identify3LetterInstruction (input, inputIndex);
        if (instrId == 0) {
          instrId = identify2LetterInstruction (input, inputIndex);
        }
      }
    }
  }

  ___BTPOP;
  return instrId;
}
