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

#ifndef ___GVASM__INSTRUCTIONS__FIXED_SIZE_INSTRUCTION_INCLUDED
#define ___GVASM__INSTRUCTIONS__FIXED_SIZE_INSTRUCTION_INCLUDED

#include "../instruction.h"

class fixedSizeInstruction_t : public instruction_t {
private:
  const int m_size;

public:
  fixedSizeInstruction_t (xValue_t* csOffset, int id, int size, operand_t* operand1 = NULL, operand_t* operand2 = NULL);

  virtual bool hasModrm (void);
  virtual int size (void);

  REFL_DECL
};

#endif
