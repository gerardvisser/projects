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

#include <lombok/base/errors.h>
#include <lombok/parsing/errorCodes.h>
#include <lombok/parsing/tokenCodes.h>
#include <lombok/parsing/tokenIDs/tokenIDs.h>
#include "groups/param.h"
#include "macroBuilder.h"

REFL_IMPL (pars::macroBuilder_t, object_t)

using namespace pars;

#define ERROR_STATE       1

#define START_STATE       10

#define FIRST_PARAM_STATE 20
#define PARAMS_STATE      21
#define PARAMS_END_STATE  22

#define END_STATE         30

macroBuilder_t::macroBuilder_t (void (* setTokenCode)(token_t*, int)) : setTokenCode (setTokenCode) {
  ___CBTPUSH;

  m_name = NULL;
  m_params = NULL;
  m_blocks = NULL;
  m_state = START_STATE;
  m_hasVarArgs = false;

  ___CBTPOP;
}

macroBuilder_t::~macroBuilder_t (void) {
  ___CBTPUSH;

  if (m_name != NULL) {
    m_name->dump ();
    if (m_blocks != NULL) {
      m_blocks->dump ();
    }
  }
  if (m_params != NULL) {
    m_params->dump ();
  }

  ___CBTPOP;
}

macro_t* macroBuilder_t::createMacro (void) {
  ___CBTPUSH;

  macro_t* result;
  if (m_state == END_STATE) {
    bool hasArgs = m_params != NULL;
    int argCount = hasArgs ? m_params->length () : 0;
    result = new macro_t (m_name, m_blocks, argCount, hasArgs, m_hasVarArgs);
    m_name = NULL;
  } else {
    result = NULL;
  }

  ___CBTPOP;
  return result;
}

void macroBuilder_t::handleEndState (token_t* token) {
  ___CBTPUSH;

  setEndStateCode (token);
  if (m_params != NULL) {
    int paramIndex = m_params->indexOf (token->text ());
    if (paramIndex > -1) {
      m_blocks->append (param_t::get (paramIndex));
    } else {
      m_blocks->appendar (token);
    }
  } else {
    m_blocks->appendar (token);
  }

  ___CBTPOP;
}

void macroBuilder_t::handleParamsEndState (token_t* token) {
  ___CBTPUSH;

  int tokenId = token->id ();
  if (tokenId == TID_S_PARENTHESIS_CLOSE) {
    m_state = END_STATE;
  } else if (tokenId == TID_S_COMMA && !m_hasVarArgs) {
    m_state = PARAMS_STATE;
  } else {
    setTokenCode (token, TCODE_ERROR);
    m_state = ERROR_STATE;
  }

  ___CBTPOP;
}

void macroBuilder_t::handleParamsState (token_t* token) {
  ___CBTPUSH;

  int tokenId = token->id ();
  if ((tokenId & MASK_BLTYPE) == BLTYPE_PRIMARY) {
    m_params->appendar (token->text ());
    m_state = PARAMS_END_STATE;
  } else if (tokenId == TID_S_ELLIPSIS) {
    m_state = PARAMS_END_STATE;
    m_hasVarArgs = true;
  } else if (tokenId == TID_S_PARENTHESIS_CLOSE && m_state == FIRST_PARAM_STATE) {
    m_state = END_STATE;
  } else {
    setTokenCode (token, TCODE_ERROR);
    m_state = ERROR_STATE;
  }

  ___CBTPOP;
}

void macroBuilder_t::handleStartState (token_t* token, listIterator_t* followingTokens) {
  ___CBTPUSH;

  if ((token->id () & MASK_BLTYPE) == BLTYPE_PRIMARY) {
    token_t* nextToken = (token_t*) followingTokens->peek ();
    if (nextToken->contextCode () == 0 && nextToken->id () == TID_S_PARENTHESIS_OPEN) {
      setTokenCode (token, TCODE_FUNC);
      m_params = new arrayList_t ();
      m_state = FIRST_PARAM_STATE;
      followingTokens->next ();
    } else {
      setTokenCode (token, TCODE_VAR);
      m_state = END_STATE;
    }
    m_name = token->text ();
    m_name->reserve ();
  } else {
    setTokenCode (token, TCODE_ERROR);
    m_state = ERROR_STATE;
  }

  ___CBTPOP;
}

bool macroBuilder_t::processToken (token_t* token, listIterator_t* followingTokens) {
  ___CBTPUSH;

  switch (m_state) {
  case START_STATE:
    handleStartState (token, followingTokens);
    break;

  case FIRST_PARAM_STATE:
  case PARAMS_STATE:
    handleParamsState (token);
    break;

  case PARAMS_END_STATE:
    handleParamsEndState (token);
    break;

  case END_STATE:
    if (m_blocks == NULL) {
      m_blocks = new arrayList_t ();
    }
    handleEndState (token);
    break;

  default: ;
  }

  ___CBTPOP;
  return m_state == ERROR_STATE;
}

void macroBuilder_t::setEndStateCode (token_t* token) {
  ___CBTPUSH;

  int code;
  int tokenId = token->id ();
  if (tokenId == BLTYPE_STRING || tokenId == BLTYPE_CHAR) {
    code = TCODE_STRING;
  } else {
    code = 0;
  }
  if (token->errorCode () > 0) {
    if (token->errorCode () == ERRC_FATAL) {
      code |= TCODE_ERROR;
    } else {
      code |= TCODE_WARN;
    }
  }
  setTokenCode (token, code);

  ___CBTPOP;
}
