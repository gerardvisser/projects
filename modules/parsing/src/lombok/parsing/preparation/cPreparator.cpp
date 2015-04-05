/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013 - 2014 Gerard Visser.

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
#include <lombok/parsing/tokenCodes.h>
#include <lombok/parsing/preparation/cPreparator.h>
#include <lombok/parsing/tokenIDs/cTokenIDs.h>
#include "groups/all.h"
#include "macroBuilder.h"

REFL_IMPL (pars::cPreparator_t, pars::preparator_t)

using namespace pars;

#define ASSEMBLE_FILENAME_STATE        1
#define DEFINE_STATE                   2
#define DIRECTIVE_IDENTIFICATION_STATE 3
#define ELIF_STATE                     4
#define IF_STATE                       5
#define IFDEF_STATE                    6
#define IGNORE_STATE                   7
#define INCLUDE_STATE                  8
#define LINE_FILENAME_STATE            9
#define LINE_NUMBER_STATE              10
#define UNDEF_STATE                    11
#define WARN_STATE                     12

static void appendSpacesToFilename (stringBuilder_t* filename, token_t* token, token_t* nextToken);

cPreparator_t::cPreparator_t (void) : m_baseDirectory (NULL), m_builder (NULL) {
  ___CBTPUSH;

  m_ifs = new arrayList_t ();
  m_expressionTokens = 0;
  m_joinLines = false;

  ___CBTPOP;
}

cPreparator_t::~cPreparator_t (void) {
  ___CBTPUSH;

  m_ifs->dump ();

  ___CBTPOP;
}

filename_t* cPreparator_t::baseDirectory (void) {
  ___CBTPUSH;

  if (m_baseDirectory == NULL) {
    m_baseDirectory = m_filename->parentDirectory ();
  }

  ___CBTPOP;
  return m_baseDirectory;
}

void cPreparator_t::cleanUp (list_t* dst) {
  ___CBTPUSH;

  returnToDefaultState (dst);
  if (m_baseDirectory != NULL) {
    m_baseDirectory->dump ();
    m_baseDirectory = NULL;
  }
  int len = m_ifs->length ();
  for (int i = 0; i < len; ++i) {
    token_t* token = (token_t*) m_ifs->removeLast ();
    setTokenCode (token, TCODE_PRE | TCODE_ERROR);
    token->dump ();
  }
  m_joinLines = false;

  ___CBTPOP;
}

void cPreparator_t::handleAssembleFilenameState (list_t* dst, token_t* token, listIterator_t* followingTokens) {
  ___CBTPUSH;

  switch (token->id ()) {
  case TID_S_GREATER:
  case TID_S_NOT_LESS: {
    dst->append (new include_t (filename_t::create (m_buffer)));
    m_state = WARN_STATE;
  } break;

  default:
    m_buffer->append (token->text ());
    appendSpacesToFilename (m_buffer, token, (token_t*) followingTokens->peek ());
  }
  setTokenCode (token, TCODE_STRING);

  ___CBTPOP;
}

void cPreparator_t::handleDefaultState (list_t* dst, token_t* token) {
  ___CBTPUSH;

  if (token->id () == TID_S_HASH) {
    if ((token->contextCode () & CTXT_NEW_LINE_BEFORE) == CTXT_NEW_LINE_BEFORE) {
      m_state = DIRECTIVE_IDENTIFICATION_STATE;
      setTokenCode (token, TCODE_PRE);
    } else {
      setTokenCode (token, TCODE_ERROR);
    }
  } else {
    dst->appendar (token);
  }

  ___CBTPOP;
}

void cPreparator_t::handleDefineState (token_t* token, listIterator_t* followingTokens) {
  ___CBTPUSH;

  bool error = ((macroBuilder_t*) m_builder)->processToken (token, followingTokens);
  if (error) {
    m_state = IGNORE_STATE;
    m_builder->dump ();
  }

  ___CBTPOP;
}

/* TODO: #import should have a warning for C and C++, but not for Objective C. */
void cPreparator_t::handleDirectiveIdentificationState (list_t* dst, token_t* token) {
  ___CBTPUSH;

  int tokenId = token->id ();
  if ((tokenId & MASK_TID_PDIR) == TID_DIRECTIVE) {
    m_directive = token;
    setTokenCode (token, TCODE_PRE);
    switch (tokenId) {
    case TID_P_DEFINE:
      m_state = DEFINE_STATE;
      m_builder = new macroBuilder_t (setTokenCode);
      break;

    case TID_P_ELIF: m_state = ELIF_STATE; break;

    case TID_P_ELSE: {
      int len = m_ifs->length ();
      if (len > 0) {
        m_ifs->setar (len - 1, m_directive);
      } else {
        setTokenCode (m_directive, TCODE_PRE | TCODE_ERROR);
      }
      m_state = WARN_STATE;
    } break;

    case TID_P_ENDIF:
      if (m_ifs->length () > 0) {
        m_ifs->eraseLast ();
      } else {
        setTokenCode (m_directive, TCODE_PRE | TCODE_ERROR);
      }
      m_state = WARN_STATE;
      break;

    case TID_P_ERROR: m_state = IGNORE_STATE; break;
    case TID_P_IF: m_state = IF_STATE; break;
    case TID_P_IFDEF: m_state = IFDEF_STATE; break;
    case TID_P_IFNDEF: m_state = IFDEF_STATE; break;
    case TID_P_INCLUDE: m_state = INCLUDE_STATE; break;
    case TID_P_LINE: m_state = LINE_NUMBER_STATE; break;
    case TID_P_PRAGMA: m_state = IGNORE_STATE; break;
    case TID_P_UNDEF: m_state = UNDEF_STATE; break;
    case TID_P_WARNING:
      setTokenCode (m_directive, TCODE_PRE | TCODE_WARN); /* Not a standard directive. */
      m_state = IGNORE_STATE;
      break;
    }
  } else {
    setTokenCode (token, TCODE_ERROR);
    m_state = IGNORE_STATE;
  }

  ___CBTPOP;
}

void cPreparator_t::handleExpressionState (token_t* token) {
  ___CBTPUSH;

  setPreprocessorExpressionCode (token);
  ++m_expressionTokens;

  ___CBTPOP;
}

void cPreparator_t::handleIfdefState (list_t* dst, token_t* token) {
  ___CBTPUSH;

  if ((token->id () & MASK_BLTYPE) == BLTYPE_PRIMARY) {
    m_state = WARN_STATE;
    m_ifs->appendar (m_directive);
  } else {
    setTokenCode (m_directive, TCODE_PRE | TCODE_ERROR);
    m_state = IGNORE_STATE;
  }

  ___CBTPOP;
}

void cPreparator_t::handleIncludeState (list_t* dst, token_t* token, listIterator_t* followingTokens) {
  ___CBTPUSH;

  int code;
  switch (token->id ()) {
  case BLTYPE_STRING:
    if (token->text ()->charAt (0) != 'L') {
      code = TCODE_STRING;
      if (token->errorCode () == 0) {
        string_t* s = token->text ();
        s = s->substring (1, s->length () - 2);
        filename_t* f = filename_t::create (s);
        s->dump ();
        dst->append (new include_t (baseDirectory ()->concat (f)));
        f->dump ();
        m_state = WARN_STATE;
      } else {
        code |= TCODE_ERROR;
        m_state = IGNORE_STATE;
      }
    } else {
      code = TCODE_ERROR;
      m_state = IGNORE_STATE;
    }
    break;

  case TID_S_LESS:
    code = TCODE_STRING;
    m_buffer->clear ();
    appendSpacesToFilename (m_buffer, token, (token_t*) followingTokens->peek ());
    m_state = ASSEMBLE_FILENAME_STATE;
    break;

  case TID_S_NOT_GREATER:
    code = TCODE_STRING;
    m_buffer->clear ();
    m_buffer->append ('=');
    appendSpacesToFilename (m_buffer, token, (token_t*) followingTokens->peek ());
    m_state = ASSEMBLE_FILENAME_STATE;
    break;

  default:
    code = TCODE_ERROR;
    m_state = IGNORE_STATE;
  }
  setTokenCode (token, code);

  ___CBTPOP;
}

void cPreparator_t::handleLineState (token_t* token) {
  ___CBTPUSH;

  int code;
  int tokenId = token->id ();
  if (m_state == LINE_NUMBER_STATE) {
    if ((tokenId == TID_N_DEC || tokenId == TID_N_OCT) && token->errorCode () != ERRC_FATAL) {
      code = token->value () > 0x7FFFFFFF ? TCODE_WARN : 0;
      m_state = LINE_FILENAME_STATE;
    } else {
      code = TCODE_ERROR;
      m_state = IGNORE_STATE;
    }
  } else if (tokenId == BLTYPE_STRING) {
    code = TCODE_STRING;
    if (token->errorCode () == ERRC_FATAL) {
      code |= TCODE_ERROR;
      m_state = IGNORE_STATE;
    } else {
      m_state = WARN_STATE;
    }
  } else {
    code = TCODE_ERROR;
    m_state = IGNORE_STATE;
  }
  setTokenCode (token, code);

  ___CBTPOP;
}

void cPreparator_t::handlePreprocessorState (list_t* dst, token_t* token, listIterator_t* followingTokens) {
  ___CBTPUSH;

  if (token->contextCode () != CTXT_NEW_LINE_BEFORE || m_joinLines) {

    if (token->id () != TID_S_BACKSLASH) {
      switch (m_state) {
      case ASSEMBLE_FILENAME_STATE:
        handleAssembleFilenameState (dst, token, followingTokens);
        break;

      case DEFINE_STATE:
        handleDefineState (token, followingTokens);
        break;

      case DIRECTIVE_IDENTIFICATION_STATE:
        handleDirectiveIdentificationState (dst, token);
        break;

      case ELIF_STATE:
      case IF_STATE:
        handleExpressionState (token);
        break;

      case IFDEF_STATE:
        handleIfdefState (dst, token);
        break;

      case INCLUDE_STATE:
        handleIncludeState (dst, token, followingTokens);
        break;

      case LINE_FILENAME_STATE:
      case LINE_NUMBER_STATE:
        handleLineState (token);
        break;

      case UNDEF_STATE:
        handleUndefState (dst, token);
        break;

      case WARN_STATE:
        setTokenCode (token, TCODE_WARN);
        break;

      default: ; /* IGNORE_STATE */
      }
      m_joinLines = false;
    } else {
      token_t* nextToken = (token_t*) followingTokens->peek ();
      int lineDiff = nextToken->beginLine () - token->endLine ();
      while (lineDiff == 1 && nextToken->id () == BLTYPE_COMMENT) {
        setTokenCode (nextToken, TCODE_COMMENT);
        token = nextToken;
        followingTokens->next ();
        nextToken = (token_t*) followingTokens->peek ();
        lineDiff += nextToken->beginLine () - token->endLine ();
      }
      if (lineDiff == 0) {
        setTokenCode (token, TCODE_ERROR);
      } else if (lineDiff > 1) {
        returnToDefaultState (dst);
      }
      m_joinLines = lineDiff == 1;
    }

  } else {

    returnToDefaultState (dst);
    handleDefaultState (dst, token);

  }

  ___CBTPOP;
}

void cPreparator_t::handleUndefState (list_t* dst, token_t* token) {
  ___CBTPUSH;

  if ((token->id () & MASK_BLTYPE) == BLTYPE_PRIMARY) {
    m_state = WARN_STATE;
  } else {
    setTokenCode (m_directive, TCODE_PRE | TCODE_ERROR);
    m_state = IGNORE_STATE;
  }

  ___CBTPOP;
}

void cPreparator_t::processToken (list_t* dst, token_t* token, listIterator_t* followingTokens) {
  ___CBTPUSH;

  if (m_state > 0) {
    handlePreprocessorState (dst, token, followingTokens);
  } else {
    handleDefaultState (dst, token);
  }

  ___CBTPOP;
}

void cPreparator_t::returnToDefaultState (list_t* dst) {
  ___CBTPUSH;

  switch (m_state) {
  case ASSEMBLE_FILENAME_STATE:
  case IFDEF_STATE:
  case INCLUDE_STATE:
  case LINE_NUMBER_STATE:
  case UNDEF_STATE:
    setTokenCode (m_directive, TCODE_PRE | TCODE_ERROR);
    break;

  case DEFINE_STATE: {
    macro_t* macro = ((macroBuilder_t*) m_builder)->createMacro ();
    if (macro != NULL) {
      dst->append (macro);
    } else {
      setTokenCode (m_directive, TCODE_PRE | TCODE_ERROR);
    }
    m_builder->dump ();
  } break;

  case ELIF_STATE: {
    int len = m_ifs->length ();
    if (len > 0 && m_expressionTokens > 0) {
      m_ifs->setar (len - 1, m_directive);
    } else {
      setTokenCode (m_directive, TCODE_PRE | TCODE_ERROR);
    }
    m_expressionTokens = 0;
  } break;

  case IF_STATE:
    if (m_expressionTokens > 0) {
      m_ifs->appendar (m_directive);
    } else {
      setTokenCode (m_directive, TCODE_PRE | TCODE_ERROR);
    }
    m_expressionTokens = 0;
    break;

  default: ;
    /* Nothing to do for:
     DIRECTIVE_IDENTIFICATION_STATE
     IGNORE_STATE
     LINE_FILENAME_STATE
     WARN_STATE */
  }
  m_state = 0;

  ___CBTPOP;
}

void cPreparator_t::setPreprocessorExpressionCode (token_t* token) {
  ___CBTPUSH;

  int code;
  int errorCode;

  switch (token->errorCode ()) {
  case ERRC_FATAL: errorCode = TCODE_ERROR; break;
  case ERRC_NONFATAL: errorCode = TCODE_WARN; break;
  default: errorCode = 0;
  }

  switch (token->id ()) {
  case BLTYPE_CHAR: code = TCODE_STRING; break;
  case BLTYPE_STRING: code = TCODE_STRING; errorCode = TCODE_ERROR; break;
  case TID_P_DEFINED: code = TCODE_PRE; break;
  default: code = 0;
  }

  setTokenCode (token, code | errorCode);

  ___CBTPOP;
}

static void appendSpacesToFilename (stringBuilder_t* filename, token_t* token, token_t* nextToken) {
  ___BTPUSH;

  if (nextToken->contextCode () == CTXT_WHITESPACE_BEFORE) {
    for (int i = 0; i < nextToken->beginColumn () - token->endColumn (); ++i) {
      filename->append (' ');
    }
  }

  ___BTPOP;
}
