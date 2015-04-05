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

#ifndef ___GVASM__ASM_TOKENISER_INCLUDED
#define ___GVASM__ASM_TOKENISER_INCLUDED

#include <lombok/parsing/tokenisers/tokeniser.h>

class asmTokeniser_t : public pars::tokeniser_t {
public:
  asmTokeniser_t (void);

protected:
  virtual pars::token_t* createSecondaryToken (pushbackStream_t* stream, int cr);
  virtual pars::token_t* nextToken (pushbackStream_t* stream, int cr);
  virtual int            readFloatSuffices (pushbackStream_t* stream, int cr);
  virtual int64_t        readHexEscapeSequence (pushbackStream_t* stream, int* errorCode);
  virtual int            readIntSuffices (pushbackStream_t* stream, int cr);
  virtual int64_t        readNonnumericEscapeSequence (pushbackStream_t* stream, int cr, int* errorCode);
  virtual void           validateCharValue (int64_t value, bool wide, int* errorCode);

  REFL_DECL
};

#endif
