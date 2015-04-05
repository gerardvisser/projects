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

#ifndef ___GVASM__BYTE_BLOCK_INCLUDED
#define ___GVASM__BYTE_BLOCK_INCLUDED

#include "expressions/xRelValue.h"

class byteBlock_t : public object_t {
public:
  typedef enum {
    TYPE_DATA,
    TYPE_INSTRUCTION
  } type_t;

private:
  xRelValue_t* const m_location;

public:
  byteBlock_t (xValue_t* csOffset);
  virtual ~byteBlock_t (void);

  virtual bool hasLocation (void);
  virtual int location (void);
  virtual int size (void) = 0;
  virtual type_t type (void) = 0;

  I_REFL_DECL

  friend class programmeBuilder_t;
};

#endif
