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

#include <lombok/parsing/tokenisers/tokens/multilineToken.h>

REFL_IMPL (pars::multilineToken_t, pars::basicToken_t)

using namespace pars;

multilineToken_t::multilineToken_t (int id, int contextCode, int errorCode, int line, int column, int length, int endLine, int endColumn) :
  basicToken_t (id, contextCode, errorCode, line, column, length), _endLine (endLine), _endColumn (endColumn) {
}

int multilineToken_t::endColumn (void) {
  return _endColumn;
}

int multilineToken_t::endLine (void) {
  return _endLine;
}
