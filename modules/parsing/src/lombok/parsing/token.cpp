/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013 Gerard Visser.

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
#include <lombok/parsing/token.h>

I_REFL_IMPL (pars::token_t, pars::block_t)

using namespace pars;

token_t::token_t (int id, int contextCode, int errorCode, int line, int column) :
  block_t (id, contextCode, errorCode), m_line (line), m_column (column), m_code (0) {
}

int token_t::beginColumn (void) {
  return m_column;
}

int token_t::beginLine (void) {
  return m_line;
}

int token_t::code (void) {
  return m_code;
}

int token_t::endColumn (void) {
  ___CBTPUSH;

  int endCol = m_column + length ();

  ___CBTPOP;
  return endCol;
}

int token_t::endLine (void) {
  return m_line;
}

string_t* token_t::text (void) {
  return NULL;
}

int64_t token_t::value (void) {
  return 0;
}
