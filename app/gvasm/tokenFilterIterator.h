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

#ifndef ___GVASM__TOKEN_FILTER_ITERATOR_INCLUDED
#define ___GVASM__TOKEN_FILTER_ITERATOR_INCLUDED

#include <lombok/parsing/token.h>
#include <lombok/util/arrayList.h>

class tokenFilterIterator_t : public object_t {
private:
  listIterator_t* m_tokens;

public:
  tokenFilterIterator_t (arrayList_t* tokens);
  virtual ~tokenFilterIterator_t (void);

  virtual bool hasNext (void);
  virtual pars::token_t* next (void);
  virtual pars::token_t* peek (void);

  REFL_DECL
};

#endif
