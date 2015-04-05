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

#ifndef ___GVASM__DATA_INCLUDED
#define ___GVASM__DATA_INCLUDED

#include <lombok/util/arrayList.h>
#include <lombok/util/charSequence.h>
#include "expressions/xTree.h"
#include "byteBlock.h"

class data_t : public byteBlock_t {
public:
  typedef enum {
    UNIT_SIZE_8_BITS = 1,
    UNIT_SIZE_16_BITS
  } unitSize_t;

private:
  typedef enum {
    STORAGE_TYPE_BYTES,
    STORAGE_TYPE_TREE,
    STORAGE_TYPE_TREE_LIST
  } storageType_t;

  typedef union {
    xTree_t* expression;
    arrayList_t* expressions;
    const char* bytes;
  } value_t;

  value_t m_value;
  storageType_t m_storageType;
  unitSize_t m_unitSize;

public:
  data_t (xValue_t* csOffset, arrayList_t* expressions, unitSize_t unitSize = UNIT_SIZE_8_BITS);
  data_t (xValue_t* csOffset, xTree_t* expression, unitSize_t unitSize = UNIT_SIZE_8_BITS);
  data_t (xValue_t* csOffset, charSequence_t* str);
  virtual ~data_t (void);

  virtual int getByte (int index);
  virtual int size (void);
  virtual type_t type (void);

  REFL_DECL
};

#endif
