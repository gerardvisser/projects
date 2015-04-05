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

#include <lombok/base/errors.h>
#include <lombok/parsing/blockTypes.h>
#include <lombok/parsing/contextCodes.h>
#include <lombok/parsing/tokenCodes.h>
#include <lombok/parsing/preparation/preparator.h>

I_REFL_IMPL (pars::preparator_t, object_t)

using namespace pars;

preparator_t::preparator_t (void) {
  ___CBTPUSH;

  m_buffer = new stringBuilder_t (256);

  ___CBTPOP;
}

preparator_t::~preparator_t (void) {
  ___CBTPUSH;

  m_buffer->dump ();

  ___CBTPOP;
}

arrayList_t* preparator_t::prepareBlocks (list_t* tokens, filename_t* filename) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (!filename->isAbsolute ()) {
    errors_printMessageAndExit ("The specified filename should be absolute");
  }
#endif

  m_state = 0;
  m_filename = filename;
  arrayList_t* result = new arrayList_t (tokens->length ());

  listIterator_t* iter = tokens->iterator ();
  token_t* token = (token_t*) iter->next ();
  while (token->contextCode () != CTXT_LAST_TOKEN) {
    int tokenId = token->id ();
    if (tokenId != BLTYPE_COMMENT) {
      processToken (result, token, iter);
    } else {
      setTokenCode (token, TCODE_COMMENT);
    }
    token = (token_t*) iter->next ();
  }
  iter->dump ();
  cleanUp (result);
  result->appendar (token);

  ___CBTPOP;
  return result;
}

void preparator_t::setTokenCode (token_t* token, int value) {
  ___CBTPUSH;

  token->m_code = value;

  ___CBTPOP;
}
