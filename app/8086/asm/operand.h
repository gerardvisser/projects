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

#ifndef ___DEBUG_ASM_OPERAND_INCLUDED
#define ___DEBUG_ASM_OPERAND_INCLUDED

#include <lombok/base/class.h>

typedef enum {
  OPERAND_TYPE_IMMEDIATE,
  OPERAND_TYPE_POINTER,
  OPERAND_TYPE_REGISTER,
  OPERAND_TYPE_SEGREGISTER
} operandType_t;

typedef enum {
  OPERAND_SIZE_UNKNOWN,
  OPERAND_SIZE_8_BITS,
  OPERAND_SIZE_16_BITS
} operandSize_t;

class operand_t : public object_t {
private:
  const operandType_t m_type;
  const int m_displacement;
  const int m_value;
  operandSize_t m_size;

public:
  operand_t (operandType_t type, int value, int displacement = 0);

  virtual int displacement (void) const;
  virtual bool isAccumulator (void) const;
  virtual bool isImmediate (void) const;
  virtual bool isPointer (void) const;
  virtual bool isRegister (void) const;
  virtual bool isSegRegister (void) const;
  virtual bool isWide (void) const;
  virtual void setSize (operandSize_t val);
  virtual operandSize_t size (void) const;
  virtual operandType_t type (void) const;
  virtual int value (void) const;

  REFL_DECL
};

#endif
