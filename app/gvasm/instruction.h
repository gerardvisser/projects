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

#ifndef ___GVASM__INSTRUCTION_INCLUDED
#define ___GVASM__INSTRUCTION_INCLUDED

#include "byteBlock.h"
#include "operand.h"

class instruction_t : public byteBlock_t {
private:
  operand_t* const m_operand1;
  operand_t* const m_operand2;
  const int m_operationCode;
  const int m_group;

public:
  instruction_t (xValue_t* csOffset, int id, operand_t* operand1, operand_t* operand2 = NULL);
  virtual ~instruction_t (void);

  virtual int group (void);
  virtual bool hasModrm (void) = 0;
  virtual operand_t* operand1 (void);
  virtual operand_t* operand2 (void);
  virtual int operationCode (void);
  virtual type_t type (void);

  I_REFL_DECL
};

#endif
