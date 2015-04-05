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

#ifndef ___LOMBOK__PARSING__TOKENISERS__TOKENS__VALUE_TEXT_TOKEN_INCLUDED
#define ___LOMBOK__PARSING__TOKENISERS__TOKENS__VALUE_TEXT_TOKEN_INCLUDED

#include <lombok/parsing/tokenisers/tokens/textToken.h>

namespace pars {

  class valueTextToken_t : public textToken_t {
  private:
    const int64_t _value;

  public:
    valueTextToken_t (int id, int contextCode, int errorCode, int line, int column, string_t* text, int64_t value);

    virtual int64_t value (void);

    REFL_DECL
  };

}

#endif
