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

#ifndef ___LOMBOK__PARSING__PREPARATION__MACRO_BUILDER_INCLUDED
#define ___LOMBOK__PARSING__PREPARATION__MACRO_BUILDER_INCLUDED

#include <lombok/parsing/token.h>
#include <lombok/util/arrayList.h>
#include "groups/macro.h"

namespace pars {

  class macroBuilder_t : public object_t {
  private:
    void (* setTokenCode)(token_t* token, int value);
    string_t* m_name;
    arrayList_t* m_params;
    list_t* m_blocks;
    int m_state;
    bool m_hasVarArgs;

  public:
    macroBuilder_t (void (* setTokenCode)(token_t*, int));
    virtual ~macroBuilder_t (void);

    macro_t* createMacro (void);
    bool     processToken (token_t* token, listIterator_t* followingTokens);

  private:
    void handleEndState (token_t* token);
    void handleParamsEndState (token_t* token);
    void handleParamsState (token_t* token);
    void handleStartState (token_t* token, listIterator_t* followingTokens);
    void setEndStateCode (token_t* token);

    REFL_DECL
  };

}

#endif
