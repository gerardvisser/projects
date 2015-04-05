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

#ifndef ___LOMBOK__PARSING__PREPARATION__C_PREPARATOR_INCLUDED
#define ___LOMBOK__PARSING__PREPARATION__C_PREPARATOR_INCLUDED

#include <lombok/parsing/preparation/preparator.h>

namespace pars {

  class cPreparator_t : public preparator_t {
  private:
    filename_t* m_baseDirectory;
    token_t* m_directive;
    object_t* m_builder;
    arrayList_t* m_ifs;
    int m_expressionTokens;
    bool m_joinLines;

  public:
    cPreparator_t (void);
    virtual ~cPreparator_t (void);

  protected:
    virtual void cleanUp (list_t* dst);
    virtual void processToken (list_t* dst, token_t* token, listIterator_t* followingTokens);

  private:
    filename_t* baseDirectory (void);
    void        handleAssembleFilenameState (list_t* dst, token_t* token, listIterator_t* followingTokens);
    void        handleDefaultState (list_t* dst, token_t* token);
    void        handleDefineState (token_t* token, listIterator_t* followingTokens);
    void        handleDirectiveIdentificationState (list_t* dst, token_t* token);
    void        handleExpressionState (token_t* token);
    void        handleIfdefState (list_t* dst, token_t* token);
    void        handleIncludeState (list_t* dst, token_t* token, listIterator_t* followingTokens);
    void        handleLineState (token_t* token);
    void        handlePreprocessorState (list_t* dst, token_t* token, listIterator_t* followingTokens);
    void        handleUndefState (list_t* dst, token_t* token);
    void        returnToDefaultState (list_t* dst);
    void        setPreprocessorExpressionCode (token_t* token);

    REFL_DECL
  };

}

#endif
