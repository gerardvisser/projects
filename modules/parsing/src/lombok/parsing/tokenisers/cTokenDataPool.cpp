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
#include <lombok/parsing/tokenIDs/cTokenIDs.h>
#include "cTokenDataPool.h"

REFL_IMPL (pars::cTokenDataPool_t, pars::tokenDataPool_t)

using namespace pars;

static tokenDataPool_t* tokenDataPool = NULL;

cTokenDataPool_t::cTokenDataPool_t (void) {
  ___CBTPUSH;

  add ("auto", TID_P_AUTO);
  add ("bool", TID_P_BOOL);
  add ("break", TID_P_BREAK);
  add ("case", TID_P_CASE);
  add ("char", TID_P_CHAR);
  add ("const", TID_P_CONST);
  add ("continue", TID_P_CONTINUE);
  add ("default", TID_P_DEFAULT);
  add ("define", TID_P_DEFINE);
  add ("defined", TID_P_DEFINED);
  add ("do", TID_P_DO);
  add ("double", TID_P_DOUBLE);
  add ("elif", TID_P_ELIF);
  add ("else", TID_P_ELSE);
  add ("endif", TID_P_ENDIF);
  add ("enum", TID_P_ENUM);
  add ("error", TID_P_ERROR);
  add ("extern", TID_P_EXTERN);
  add ("false", TID_P_FALSE);
  add ("float", TID_P_FLOAT);
  add ("for", TID_P_FOR);
  add ("fortran", TID_P_FORTRAN);
  add ("goto", TID_P_GOTO);
  add ("if", TID_P_IF);
  add ("ifdef", TID_P_IFDEF);
  add ("ifndef", TID_P_IFNDEF);
  add ("include", TID_P_INCLUDE);
  add ("int", TID_P_INT);
  add ("line", TID_P_LINE);
  add ("long", TID_P_LONG);
  add ("NULL", TID_P_NULL);
  add ("pragma", TID_P_PRAGMA);
  add ("register", TID_P_REGISTER);
  add ("restrict", TID_P_RESTRICT);
  add ("return", TID_P_RETURN);
  add ("short", TID_P_SHORT);
  add ("signed", TID_P_SIGNED);
  add ("sizeof", TID_P_SIZEOF);
  add ("static", TID_P_STATIC);
  add ("struct", TID_P_STRUCT);
  add ("switch", TID_P_SWITCH);
  add ("true", TID_P_TRUE);
  add ("typedef", TID_P_TYPEDEF);
  add ("undef", TID_P_UNDEF);
  add ("union", TID_P_UNION);
  add ("unsigned", TID_P_UNSIGNED);
  add ("void", TID_P_VOID);
  add ("volatile", TID_P_VOLATILE);
  add ("warning", TID_P_WARNING);
  add ("while", TID_P_WHILE);

  ___CBTPOP;
}

tokenDataPool_t* cTokenDataPool_t::instance (void) {
  ___CBTPUSH;

  if (tokenDataPool == NULL) {
    tokenDataPool = new cTokenDataPool_t ();
    objectReleasePool_t::instance ()->add (tokenDataPool);
  }

  ___CBTPOP;
  return tokenDataPool;
}
