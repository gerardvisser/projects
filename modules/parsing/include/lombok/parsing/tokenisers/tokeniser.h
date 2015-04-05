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

#ifndef ___LOMBOK__PARSING__TOKENISERS__TOKENISER_INCLUDED
#define ___LOMBOK__PARSING__TOKENISERS__TOKENISER_INCLUDED

#include <lombok/io/pushbackStream.h>
#include <lombok/parsing/token.h>
#include <lombok/parsing/tokenisers/tokenDataPool.h>
#include <lombok/util/arrayList.h>
#include <lombok/util/stringBuilder.h>

namespace pars {

  class tokeniser_t : public object_t {
  protected:
    tokenDataPool_t* const tokenDataPool;
    stringBuilder_t* buffer;
    int contextCode;
    int column;
    int line;

  public:
    tokeniser_t (tokenDataPool_t* tokenDataPool);
    virtual ~tokeniser_t (void);

    virtual arrayList_t* getTokens (pushbackStream_t* stream);

  protected:
    virtual token_t* createCharToken (pushbackStream_t* stream, bool wide = false);
    virtual token_t* createCommentToken (pushbackStream_t* stream, int cr);
    virtual token_t* createNumberToken (pushbackStream_t* stream, int cr);
    virtual token_t* createPrimaryToken (pushbackStream_t* stream, int cr);
    virtual token_t* createSecondaryToken (pushbackStream_t* stream, int cr) = 0;
    virtual token_t* createStringToken (pushbackStream_t* stream, bool wide = false);
    virtual void     findEndOfCharToken (pushbackStream_t* stream, int cr, int* errorCode);
    virtual bool     isIdentifierChar (int cr);
    virtual bool     isIdentifierFirstChar (int cr);
    virtual bool     isSpace (int cr);
    virtual token_t* nextToken (pushbackStream_t* stream, int cr) = 0;
    virtual int      readFloatSuffices (pushbackStream_t* stream, int cr) = 0;
    virtual int64_t  readHexEscapeSequence (pushbackStream_t* stream, int* errorCode) = 0;
    virtual int      readIntSuffices (pushbackStream_t* stream, int cr) = 0;
    virtual int64_t  readNonnumericEscapeSequence (pushbackStream_t* stream, int cr, int* errorCode) = 0;
    virtual int64_t  readOctalEscapeSequence (pushbackStream_t* stream, int cr, int* errorCode);
    virtual int64_t  readUniversalCharacterName (pushbackStream_t* stream, int* errorCode);
    virtual void     resetState (void);
    virtual int      skipSpaces (pushbackStream_t* stream, int cr);
    virtual void     validateCharValue (int64_t value, bool wide, int* errorCode) = 0;

  private:
    int64_t readDecValue (int* errorCode);
    int64_t readOctValue (int* errorCode);

    I_REFL_DECL
  };

}

#endif
