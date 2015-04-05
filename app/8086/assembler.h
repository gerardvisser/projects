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

#ifndef ___DEBUG_ASSEMBLER_INCLUDED
#define ___DEBUG_ASSEMBLER_INCLUDED

#include "asm/compiler.h"
#include "asm/tokenizer.h"
#include "lineInputReader.h"
#include "asm/instructionBuilder.h"

class assembler_t : public object_t {
private:
  instructionBuilder_t* m_instructionBuilder;
  lineInputReader_t* m_inputReader;
  tokenizer_t* m_tokenizer;
  compiler_t* m_compiler;
  char* m_input;
  int m_inputSize;

public:
  assembler_t (memory_t* memory, lineInputReader_t* inputReader);
  virtual ~assembler_t (void);

  virtual void assemble (address_t* address);

private:
  void printErrorMessage (error_t* error);

  REFL_DECL
};

#endif
