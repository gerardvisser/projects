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

#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include <lombok/base/mem.h>
#include <lombok/base/errors.h>

REFL_IMPL (assembler_t, object_t)

assembler_t::assembler_t (memory_t* memory, lineInputReader_t* inputReader) : m_inputReader (inputReader) {
  ___CBTPUSH;

  m_inputSize = 256;
  inputReader->reserve ();
  m_tokenizer = new tokenizer_t ();
  m_compiler = new compiler_t (memory);
  m_instructionBuilder = new instructionBuilder_t ();
  m_input = (char*) mem_allocate ("char[]", m_inputSize, PTYPE_INTERNAL);

  ___CBTPOP;
}

assembler_t::~assembler_t (void) {
  ___CBTPUSH;

  m_instructionBuilder->dump ();
  m_inputReader->dump ();
  m_tokenizer->dump ();
  m_compiler->dump ();
  mem_release (m_input);

  ___CBTPOP;
}

void assembler_t::assemble (address_t* address) {
  ___CBTPUSH;

  error_t error;
  bool done = false;
  while (!done) {
    printf ("%04X:%04X ", address->segment (), address->offset ());
    m_inputReader->getLineFromStdin ();
    m_input = m_inputReader->readAll (m_input, &m_inputSize);
    done = m_input[0] == 0;
    if (!done) {
      list_t* tokens = m_tokenizer->getTokens (&error, m_input);
      if (error.code == 0) {
        instruction_t* instruction = m_instructionBuilder->getInstruction (&error, tokens, address);
        if (error.code == 0) {
          m_compiler->compile (address, instruction);
          instruction->dump ();
        } else {
          printErrorMessage (&error);
        }
      } else {
        printErrorMessage (&error);
      }
      tokens->dump ();
    }
  }

  ___CBTPOP;
}

void assembler_t::printErrorMessage (error_t* error) {
  ___CBTPUSH;

  char padding[256];
  m_inputReader->rewindEntirely ();
  m_inputReader->skip (error->location);
  int screenLocation = m_inputReader->position () + 10;
  if (screenLocation < 256) {
    memset (padding, ' ', screenLocation);
    padding[screenLocation] = 0;
    printf ("%s^ Error\n", padding);
  } else {
    printf ("An error occurred.\n");
  }

  ___CBTPOP;
}
