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
#include <lombok/base/objectReleasePool.h>
#include "asmTokenDataPool.h"
#include "asmTokenIDs.h"

REFL_IMPL (asmTokenDataPool_t, pars::tokenDataPool_t)

static pars::tokenDataPool_t* tokenDataPool = NULL;

asmTokenDataPool_t::asmTokenDataPool_t (void) {
  ___CBTPUSH;

  add ("aaa", TID_P_AAA);
  add ("aad", TID_P_AAD);
  add ("aam", TID_P_AAM);
  add ("aas", TID_P_AAS);
  add ("adc", TID_P_ADC);
  add ("add", TID_P_ADD);
  add ("ah", TID_P_AH);
  add ("al", TID_P_AL);
  add ("and", TID_P_AND);
  add ("ax", TID_P_AX);
  add ("bh", TID_P_BH);
  add ("bl", TID_P_BL);
  add ("bp", TID_P_BP);
  add ("bx", TID_P_BX);
  add ("byte", TID_P_BYTE);
  add ("call", TID_P_CALL);
  add ("cbw", TID_P_CBW);
  add ("ch", TID_P_CH);
  add ("cl", TID_P_CL);
  add ("clc", TID_P_CLC);
  add ("cld", TID_P_CLD);
  add ("cli", TID_P_CLI);
  add ("cmc", TID_P_CMC);
  add ("cmp", TID_P_CMP);
  add ("cmpsb", TID_P_CMPSB);
  add ("cmpsw", TID_P_CMPSW);
  add ("cs", TID_P_CS);
  add ("CS", TID_P_CS_L);
  add ("cwd", TID_P_CWD);
  add ("cx", TID_P_CX);
  add ("daa", TID_P_DAA);
  add ("das", TID_P_DAS);
  add ("db", TID_P_DB);
  add ("dec", TID_P_DEC);
  add ("dh", TID_P_DH);
  add ("di", TID_P_DI);
  add ("div", TID_P_DIV);
  add ("dl", TID_P_DL);
  add ("ds", TID_P_DS);
  add ("DS", TID_P_DS_L);
  add ("dw", TID_P_DW);
  add ("dx", TID_P_DX);
  add ("es", TID_P_ES);
  add ("far", TID_P_FAR);
  add ("hlt", TID_P_HLT);
  add ("idiv", TID_P_IDIV);
  add ("imul", TID_P_IMUL);
  add ("in", TID_P_IN);
  add ("inc", TID_P_INC);
  add ("int", TID_P_INT);
  add ("into", TID_P_INTO);
  add ("iret", TID_P_IRET);
  add ("ja", TID_P_JA);
  add ("jae", TID_P_JAE);
  add ("jb", TID_P_JB);
  add ("jbe", TID_P_JBE);
  add ("jc", TID_P_JC);
  add ("jcxz", TID_P_JCXZ);
  add ("je", TID_P_JE);
  add ("jg", TID_P_JG);
  add ("jge", TID_P_JGE);
  add ("jl", TID_P_JL);
  add ("jle", TID_P_JLE);
  add ("jmp", TID_P_JMP);
  add ("jna", TID_P_JNA);
  add ("jnae", TID_P_JNAE);
  add ("jnb", TID_P_JNB);
  add ("jnbe", TID_P_JNBE);
  add ("jnc", TID_P_JNC);
  add ("jne", TID_P_JNE);
  add ("jng", TID_P_JNG);
  add ("jnge", TID_P_JNGE);
  add ("jnl", TID_P_JNL);
  add ("jnle", TID_P_JNLE);
  add ("jno", TID_P_JNO);
  add ("jnp", TID_P_JNP);
  add ("jns", TID_P_JNS);
  add ("jnz", TID_P_JNZ);
  add ("jo", TID_P_JO);
  add ("jp", TID_P_JP);
  add ("jpe", TID_P_JPE);
  add ("jpo", TID_P_JPO);
  add ("js", TID_P_JS);
  add ("jz", TID_P_JZ);
  add ("lahf", TID_P_LAHF);
  add ("lds", TID_P_LDS);
  add ("lea", TID_P_LEA);
  add ("les", TID_P_LES);
  add ("lock", TID_P_LOCK);
  add ("lodsb", TID_P_LODSB);
  add ("lodsw", TID_P_LODSW);
  add ("loop", TID_P_LOOP);
  add ("loope", TID_P_LOOPE);
  add ("loopne", TID_P_LOOPNE);
  add ("loopnz", TID_P_LOOPNZ);
  add ("loopz", TID_P_LOOPZ);
  add ("mov", TID_P_MOV);
  add ("movsb", TID_P_MOVSB);
  add ("movsw", TID_P_MOVSW);
  add ("mul", TID_P_MUL);
  add ("neg", TID_P_NEG);
  add ("nop", TID_P_NOP);
  add ("not", TID_P_NOT);
  add ("or", TID_P_OR);
  add ("out", TID_P_OUT);
  add ("pop", TID_P_POP);
  add ("popf", TID_P_POPF);
  add ("ptr", TID_P_PTR);
  add ("push", TID_P_PUSH);
  add ("pushf", TID_P_PUSHF);
  add ("rcl", TID_P_RCL);
  add ("rcr", TID_P_RCR);
  add ("rep", TID_P_REP);
  add ("repe", TID_P_REPE);
  add ("repne", TID_P_REPNE);
  add ("repnz", TID_P_REPNZ);
  add ("repz", TID_P_REPZ);
  add ("ret", TID_P_RET);
  add ("retf", TID_P_RETF);
  add ("rol", TID_P_ROL);
  add ("ror", TID_P_ROR);
  add ("sahf", TID_P_SAHF);
  add ("sal", TID_P_SAL);
  add ("sar", TID_P_SAR);
  add ("sbb", TID_P_SBB);
  add ("scasb", TID_P_SCASB);
  add ("scasw", TID_P_SCASW);
  add ("shl", TID_P_SHL);
  add ("shr", TID_P_SHR);
  add ("si", TID_P_SI);
  add ("sp", TID_P_SP);
  add ("ss", TID_P_SS);
  add ("stc", TID_P_STC);
  add ("std", TID_P_STD);
  add ("sti", TID_P_STI);
  add ("stosb", TID_P_STOSB);
  add ("stosw", TID_P_STOSW);
  add ("sub", TID_P_SUB);
  add ("test", TID_P_TEST);
  add ("wait", TID_P_WAIT);
  add ("word", TID_P_WORD);
  add ("xchg", TID_P_XCHG);
  add ("xlat", TID_P_XLAT);
  add ("xor", TID_P_XOR);

  ___CBTPOP;
}

pars::tokenDataPool_t* asmTokenDataPool_t::instance (void) {
  ___CBTPUSH;

  if (tokenDataPool == NULL) {
    tokenDataPool = new asmTokenDataPool_t ();
    objectReleasePool_t::instance ()->add (tokenDataPool);
  }

  ___CBTPOP;
  return tokenDataPool;
}
