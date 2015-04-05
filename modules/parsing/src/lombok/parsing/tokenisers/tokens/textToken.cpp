/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013, 2014 Gerard Visser.

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
#include <lombok/parsing/tokenisers/tokens/textToken.h>

REFL_IMPL (pars::textToken_t, pars::token_t)

using namespace pars;

textToken_t::textToken_t (int id, int contextCode, int errorCode, int line, int column, string_t* text) : token_t (id, contextCode, errorCode, line, column), _text (text) {
  ___CBTPUSH;

  text->reserve ();

  ___CBTPOP;
}

textToken_t::~textToken_t (void) {
  ___CBTPUSH;

  _text->dump ();

  ___CBTPOP;
}

int textToken_t::length (void) {
  ___CBTPUSH;

  int len = _text->length ();

  ___CBTPOP;
  return len;
}

string_t* textToken_t::text (void) {
  return _text;
}
