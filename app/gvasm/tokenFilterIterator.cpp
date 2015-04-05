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
#include "tokenFilterIterator.h"
#include "asmTokenIDs.h"

REFL_IMPL (tokenFilterIterator_t, object_t)

using namespace pars;

static void moveToNextElementToReturn (listIterator_t* tokens);

tokenFilterIterator_t::tokenFilterIterator_t (arrayList_t* tokens) {
  ___CBTPUSH;

  m_tokens = tokens->iterator ();
  moveToNextElementToReturn (m_tokens);

  ___CBTPOP;
}

tokenFilterIterator_t::~tokenFilterIterator_t (void) {
  ___CBTPUSH;

  m_tokens->dump ();

  ___CBTPOP;
}

bool tokenFilterIterator_t::hasNext (void) {
  ___CBTPUSH;

  bool result = m_tokens->hasNext ();

  ___CBTPOP;
  return result;
}

token_t* tokenFilterIterator_t::next (void) {
  ___CBTPUSH;

  token_t* token = (token_t*) m_tokens->next ();
  moveToNextElementToReturn (m_tokens);

  ___CBTPOP;
  return token;
}

token_t* tokenFilterIterator_t::peek (void) {
  ___CBTPUSH;

  token_t* token = (token_t*) m_tokens->peek ();

  ___CBTPOP;
  return token;
}

static void moveToNextElementToReturn (listIterator_t* tokens) {
  ___BTPUSH;

  while (tokens->hasNext () && ((token_t*) tokens->peek ())->id () == BLTYPE_COMMENT) {
    tokens->next ();
  }

  ___BTPOP;
}
