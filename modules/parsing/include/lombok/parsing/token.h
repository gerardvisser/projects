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

#ifndef ___LOMBOK__PARSING__TOKEN_INCLUDED
#define ___LOMBOK__PARSING__TOKEN_INCLUDED

#include <stdint.h>
#include <lombok/parsing/block.h>
#include <lombok/util/string.h>

namespace pars {

  class token_t : public block_t {
  private:
    const int m_column;
    const int m_line;
    int m_code;

  public:
    token_t (int id, int contextCode, int errorCode, int line, int column);

    virtual int       beginColumn (void);
    virtual int       beginLine (void);
    virtual int       code (void);
    virtual int       endColumn (void);
    virtual int       endLine (void);
    virtual int       length (void) = 0;
    virtual string_t* text (void);
    virtual int64_t   value (void);

    I_REFL_DECL

    friend class preparator_t;
  };

}

#endif
