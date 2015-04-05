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

#include <lombok/base/all.h>
#include <lombok/codepainter/codepainter.h>
#include <lombok/io/all.h>
#include <lombok/parsing/all.h>
#include <lombok/parsing/preparation/cPreparator.h>
#include <lombok/parsing/tokenisers/all.h>
#include <lombok/util/all.h>

using namespace pars;

int main (int argc, char** args, char** env) {
  ___BTPUSH;
  errors_printBacktraceFatalSignals ();

  filename_t* inputFile = filename_t::create ("/usr/include/stdio.h");
  const char* const outputFile = "stdio.html";

  tokeniser_t* tokeniser = new cTokeniser_t ();
  pushbackStream_t* instream = new pushbackStream_t (new fileInputStream_t (inputFile->get ()));

  list_t* tokens = tokeniser->getTokens (instream);

  preparator_t* preparator = new cPreparator_t ();
  arrayList_t* preparedBlocks = preparator->prepareBlocks (tokens, inputFile);
  preparedBlocks->dump ();
  preparator->dump ();

  codepainter_createHtml (outputFile, tokens);

  tokens->dump ();
  instream->dump ();
  tokeniser->dump ();
  inputFile->dump ();

  objectReleasePool_t::instance ()->dump ();
  mem_printExitStats ();
  ___BTPOP;
  return 0;
}
