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
#include <lombok/parsing/tokenIDs/javaTokenIDs.h>
#include "javaTokenDataPool.h"

REFL_IMPL (pars::javaTokenDataPool_t, pars::tokenDataPool_t)

using namespace pars;

static tokenDataPool_t* tokenDataPool = NULL;

javaTokenDataPool_t::javaTokenDataPool_t (void) {
  ___CBTPUSH;

  add ("abstract", TID_P_ABSTRACT);
  add ("assert", TID_P_ASSERT);
  add ("boolean", TID_P_BOOLEAN);
  add ("break", TID_P_BREAK);
  add ("byte", TID_P_BYTE);
  add ("case", TID_P_CASE);
  add ("catch", TID_P_CATCH);
  add ("char", TID_P_CHAR);
  add ("class", TID_P_CLASS);
  add ("const", TID_P_CONST);
  add ("continue", TID_P_CONTINUE);
  add ("default", TID_P_DEFAULT);
  add ("do", TID_P_DO);
  add ("double", TID_P_DOUBLE);
  add ("else", TID_P_ELSE);
  add ("enum", TID_P_ENUM);
  add ("extends", TID_P_EXTENDS);
  add ("false", TID_P_FALSE);
  add ("final", TID_P_FINAL);
  add ("finally", TID_P_FINALLY);
  add ("float", TID_P_FLOAT);
  add ("for", TID_P_FOR);
  add ("goto", TID_P_GOTO);
  add ("if", TID_P_IF);
  add ("implements", TID_P_IMPLEMENTS);
  add ("import", TID_P_IMPORT);
  add ("instanceof", TID_P_INSTANCEOF);
  add ("int", TID_P_INT);
  add ("interface", TID_P_INTERFACE);
  add ("long", TID_P_LONG);
  add ("native", TID_P_NATIVE);
  add ("new", TID_P_NEW);
  add ("null", TID_P_NULL);
  add ("package", TID_P_PACKAGE);
  add ("private", TID_P_PRIVATE);
  add ("protected", TID_P_PROTECTED);
  add ("public", TID_P_PUBLIC);
  add ("return", TID_P_RETURN);
  add ("short", TID_P_SHORT);
  add ("static", TID_P_STATIC);
  add ("strictfp", TID_P_STRICTFP);
  add ("super", TID_P_SUPER);
  add ("switch", TID_P_SWITCH);
  add ("synchronized", TID_P_SYNCHRONIZED);
  add ("this", TID_P_THIS);
  add ("throw", TID_P_THROW);
  add ("throws", TID_P_THROWS);
  add ("transient", TID_P_TRANSIENT);
  add ("true", TID_P_TRUE);
  add ("try", TID_P_TRY);
  add ("void", TID_P_VOID);
  add ("volatile", TID_P_VOLATILE);
  add ("while", TID_P_WHILE);

  ___CBTPOP;
}

tokenDataPool_t* javaTokenDataPool_t::instance (void) {
  ___CBTPUSH;

  if (tokenDataPool == NULL) {
    tokenDataPool = new javaTokenDataPool_t ();
    objectReleasePool_t::instance ()->add (tokenDataPool);
  }

  ___CBTPOP;
  return tokenDataPool;
}
