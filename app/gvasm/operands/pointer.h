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

#ifndef ___GVASM__OPERANDS__POINTER_INCLUDED
#define ___GVASM__OPERANDS__POINTER_INCLUDED

#include "../operand.h"
#include "../expressions/xTree.h"

namespace ops {

  class pointer_t : public operand_t {
  private:
    union {
      xTree_t* tree;
      int64_t integer;
    } m_displacement;

    bool m_hasTreeDisplacement;
    const int m_modrmValue;

  public:
    pointer_t (int modrmValue, xTree_t* displacement, operandSize_t size);
    virtual ~pointer_t (void);

    virtual int displacement (void) const;
    virtual operandType_t type (void) const;
    virtual int value (void) const;

    REFL_DECL
  };

}

#endif
