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

#include <lombok/base/errors.h>
#include <lombok/parsing/contextCodes.h>
#include <lombok/parsing/errorCodes.h>
#include <lombok/parsing/tokenisers/tokens/multilineTextToken.h>
#include <lombok/parsing/tokenisers/tokens/textToken.h>
#include "asmErrors.h"
#include "asmPreparator.h"
#include "asmTokenIDs.h"
#include "tokenFilterIterator.h"

REFL_IMPL (asmPreparator_t, object_t)

using namespace pars;

static token_t* changePrimaryTokenId (token_t* token, int id);
static token_t* createDefineStringToken (token_t* dbToken);

asmPreparator_t::asmPreparator_t (void) {
  ___CBTPUSH;

  m_tokenText = new stringBuilder_t (256);

  ___CBTPOP;
}

asmPreparator_t::~asmPreparator_t (void) {
  ___CBTPUSH;

  m_tokenText->dump ();

  ___CBTPOP;
}

token_t* asmPreparator_t::mergeTokens (int id, token_t* token, token_t* subsequentToken) {
  ___CBTPUSH;

  const int contextCode = token->contextCode ();
  const int line = token->beginLine ();
  const int column = token->beginColumn ();
  const int endLine = subsequentToken->endLine ();
  const int endColumn = subsequentToken->endColumn ();

  m_tokenText->append (token->text ());
  if (subsequentToken->beginLine () != line) {
    m_tokenText->append ('\n', subsequentToken->endLine () - line);
    m_tokenText->append (' ', subsequentToken->beginColumn () - 1);
  } else {
    m_tokenText->append (' ', subsequentToken->beginColumn () - column);
  }
  m_tokenText->append (subsequentToken->text ());

  string_t* text = m_tokenText->toString ();
  token_t* result = new multilineTextToken_t (id, contextCode, 0, line, column, text, endLine, endColumn);
  m_tokenText->clear ();
  text->dump ();

  ___CBTPOP;
  return result;
}

arrayList_t* asmPreparator_t::prepareTokens (arrayList_t* tokens) {
  ___CBTPUSH;

  tokenFilterIterator_t iter (tokens);
  arrayList_t* result = new arrayList_t (tokens->length ());

  token_t* token = iter.next ();
  while (token->contextCode () != CTXT_LAST_TOKEN) {
    if (token->errorCode () != ERRC_FATAL) {
      bool tokenToBeReserved = true;
      switch (token->id ()) {
      case TID_N_FLOAT:
        asmErrors::exit ("floating point numbers cannot be used", token);
        break;

      case TID_P_FAR:
        asmErrors::exit ("‘call’ or ‘jmp’ expected before ‘far’ token", token);
        break;

      case TID_P_PTR:
        asmErrors::exit ("‘byte’ or ‘word’ expected before ‘ptr’ token", token);
        break;

      case TID_P_BYTE:
        if (iter.peek ()->id () == TID_P_PTR) {
          token = mergeTokens (TID_CAT_PTRDETERMINER, token, iter.next ());
          tokenToBeReserved = false;
        } else {
          asmErrors::exit ("‘ptr’ expected after ‘byte’ token", token);
        }
        break;

      case TID_P_WORD:
        if (iter.peek ()->id () == TID_P_PTR) {
          token = mergeTokens (TID_CAT_PTRDETERMINER | MASK_TID_PWIDE, token, iter.next ());
          tokenToBeReserved = false;
        } else {
          asmErrors::exit ("‘ptr’ expected after ‘word’ token", token);
        }
        break;

      case TID_P_CALL:
        if (iter.peek ()->id () == TID_P_FAR) {
          token = mergeTokens (TID_P_CALLF, token, iter.next ());
          tokenToBeReserved = false;
        }
        break;

      case TID_P_JMP:
        if (iter.peek ()->id () == TID_P_FAR) {
          token = mergeTokens (TID_P_JMPF, token, iter.next ());
          tokenToBeReserved = false;
        }
        break;

      case TID_P_ES:
        if (iter.peek ()->id () == TID_S_COLON) {
          token = mergeTokens (TID_P_ES_O, token, iter.next ());
          tokenToBeReserved = false;
        }
        break;

      case TID_P_CS:
        if (iter.peek ()->id () == TID_S_COLON) {
          token = mergeTokens (TID_P_CS_O, token, iter.next ());
          tokenToBeReserved = false;
        }
        break;

      case TID_P_SS:
        if (iter.peek ()->id () == TID_S_COLON) {
          token = mergeTokens (TID_P_SS_O, token, iter.next ());
          tokenToBeReserved = false;
        }
        break;

      case TID_P_DS:
        if (iter.peek ()->id () == TID_S_COLON) {
          token = mergeTokens (TID_P_DS_O, token, iter.next ());
          tokenToBeReserved = false;
        }
        break;

      case BLTYPE_PRIMARY:
        if (iter.peek ()->id () == TID_S_COLON) {
          token = changePrimaryTokenId (token, TID_CAT_LABEL);
          tokenToBeReserved = false;
          iter.next ();
        }
        break;

      case TID_P_CS_L:
      case TID_P_DS_L:
        if (iter.peek ()->id () != TID_S_COLON) {
          token = changePrimaryTokenId (token, BLTYPE_PRIMARY);
          tokenToBeReserved = false;
        } else {
          iter.next ();
        }
        break;

      case TID_P_DB:
        if (iter.peek ()->id () == BLTYPE_STRING) {
          token = createDefineStringToken (token);
          tokenToBeReserved = false;
        }
        break;
      }
      if (tokenToBeReserved) {
        token->reserve ();
      }
      result->append (token);
      token = iter.next ();
    } else {
      asmErrors::exit ("error in token", token);
    }
  }
  result->appendar (token);

  ___CBTPOP;
  return result;
}

static token_t* changePrimaryTokenId (token_t* token, int id) {
  ___BTPUSH;

  const int contextCode = token->contextCode ();
  const int line = token->beginLine ();
  const int column = token->beginColumn ();
  string_t* const text = token->text ();
  token_t* result = new textToken_t (id, contextCode, 0, line, column, text);

  ___BTPOP;
  return result;
}

static token_t* createDefineStringToken (token_t* dbToken) {
  ___BTPUSH;

  const int contextCode = dbToken->contextCode ();
  const int errorCode = dbToken->errorCode ();
  const int line = dbToken->beginLine ();
  const int column = dbToken->beginColumn ();
  string_t* const text = dbToken->text ();
  token_t* result = new textToken_t (TID_P_DC, contextCode, errorCode, line, column, text);

  ___BTPOP;
  return result;
}
