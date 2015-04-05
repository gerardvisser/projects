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

#ifndef ___GVASM__ASM_PREPARATOR_INCLUDED
#define ___GVASM__ASM_PREPARATOR_INCLUDED

#include <lombok/parsing/token.h>
#include <lombok/util/arrayList.h>
#include <lombok/util/stringBuilder.h>

class asmPreparator_t : public object_t {
private:
  stringBuilder_t* m_tokenText;

public:
  asmPreparator_t (void);
  virtual ~asmPreparator_t (void);

  virtual arrayList_t* prepareTokens (arrayList_t* tokens);

private:
  pars::token_t* mergeTokens (int id, pars::token_t* token, pars::token_t* subsequentToken);

  REFL_DECL
};

#endif
