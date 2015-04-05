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

#ifndef ___LOMBOK__PARSING__PREPARATION__PREPARATOR_INCLUDED
#define ___LOMBOK__PARSING__PREPARATION__PREPARATOR_INCLUDED

#include <lombok/io/filename.h>
#include <lombok/parsing/token.h>
#include <lombok/util/arrayList.h>
#include <lombok/util/stringBuilder.h>

namespace pars {

  class preparator_t : public object_t {
  protected:
    stringBuilder_t* m_buffer;
    filename_t* m_filename; /* weak */
    int m_state;

  public:
    preparator_t (void);
    virtual ~preparator_t (void);

    virtual arrayList_t* prepareBlocks (list_t* tokens, filename_t* filename);

  protected:
    virtual void cleanUp (list_t* dst) = 0;
    virtual void processToken (list_t* dst, token_t* token, listIterator_t* followingTokens) = 0;

    static void setTokenCode (token_t* token, int value);

    I_REFL_DECL
  };

}

#endif
