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

#ifndef ___LOMBOK__PARSING__PREPARATION__GROUPS__MACRO_INCLUDED
#define ___LOMBOK__PARSING__PREPARATION__GROUPS__MACRO_INCLUDED

#include <lombok/parsing/block.h>
#include <lombok/util/list.h>
#include <lombok/util/string.h>

namespace pars {

  class macro_t : public block_t {
  private:
    string_t* const m_name;
    list_t* const m_blocks;
    const int m_argCount;
    const int m_hasArgs;

  public:
    macro_t (string_t* name, list_t* blocks, int argCount = 0, bool hasArgs = false, bool hasVarArgs = false);
    virtual ~macro_t (void);

    virtual int       argCount (void);
    virtual bool      hasArgs (void);
    virtual bool      hasVarArgs (void);
    virtual string_t* name (void);
    virtual list_t*   replacement (void);

    REFL_DECL
  };

}

#endif
