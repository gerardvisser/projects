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

#ifndef ___DEBUG_ASM_TOKENIZER_INCLUDED
#define ___DEBUG_ASM_TOKENIZER_INCLUDED

#include "error.h"
#include "token.h"
#include <lombok/util/list.h>

class tokenizer_t : public object_t {
private:
  int m_inputIndex;
  int m_errorCode;

public:
  tokenizer_t (void);
  virtual ~tokenizer_t (void);

  virtual list_t* getTokens (error_t* error, const char* input);

private:
  token_t* nextToken (const char* input);
  void     reset (void);
  token_t* tryCreatingCharToken (const char* input);
  token_t* tryCreatingNumberToken (const char* input);
  token_t* tryCreatingPointerDeterminerToken (const char* input);
  token_t* tryCreatingRegisterToken (const char* input);

  REFL_DECL
};

#endif
