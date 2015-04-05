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

#include <string.h>
#include <lombok/base/errors.h>
#include <lombok/base/mem.h>
#include "data.h"

REFL_IMPL (data_t, byteBlock_t)

data_t::data_t (xValue_t* csOffset, arrayList_t* expressions, unitSize_t unitSize) : byteBlock_t (csOffset) {
  ___CBTPUSH;

  m_storageType = STORAGE_TYPE_TREE_LIST;
  m_value.expressions = expressions;
  m_unitSize = unitSize;

  ___CBTPOP;
}

data_t::data_t (xValue_t* csOffset, xTree_t* expression, unitSize_t unitSize) : byteBlock_t (csOffset) {
  ___CBTPUSH;

  m_storageType = STORAGE_TYPE_TREE;
  m_value.expression = expression;
  m_unitSize = unitSize;

  ___CBTPOP;
}

data_t::data_t (xValue_t* csOffset, charSequence_t* str) : byteBlock_t (csOffset) {
  ___CBTPUSH;

  m_storageType = STORAGE_TYPE_BYTES;
  m_value.bytes = str->toUtf8 ();
  m_unitSize = UNIT_SIZE_8_BITS;

  ___CBTPOP;
}

data_t::~data_t (void) {
  ___CBTPUSH;

  switch (m_storageType) {
  case STORAGE_TYPE_BYTES:
    mem_release ((void*) m_value.bytes); 
    break;

  case STORAGE_TYPE_TREE:
    m_value.expression->dump ();
    break;

  case STORAGE_TYPE_TREE_LIST:
    m_value.expressions->dump ();
    break;
  }

  ___CBTPOP;
}

int data_t::getByte (int index) {
  ___CBTPUSH;

  int result;
  switch (m_storageType) {
  case STORAGE_TYPE_BYTES:
    result = (unsigned char) m_value.bytes[index];
    break;

  case STORAGE_TYPE_TREE:
    result = m_value.expression->value ();
    if (m_unitSize == UNIT_SIZE_16_BITS && (index & 1) != 0) {
      result >>= 8;
    }
    result &= 0xFF;
    break;

  case STORAGE_TYPE_TREE_LIST:
    if (m_unitSize == UNIT_SIZE_8_BITS) {
      xTree_t* expression = (xTree_t*) m_value.expressions->get (index);
      result = expression->value ();
    } else {
      xTree_t* expression = (xTree_t*) m_value.expressions->get (index / 2);
      result = expression->value ();
      if (index & 1) {
        result >>= 8;
      }
    }
    result &= 0xFF;
    break;
  }

  ___CBTPOP;
  return result;
}

int data_t::size (void) {
  ___CBTPUSH;

  int result;
  switch (m_storageType) {
  case STORAGE_TYPE_BYTES:
    result = strlen (m_value.bytes);
    break;

  case STORAGE_TYPE_TREE:
    result = m_unitSize;
    break;

  case STORAGE_TYPE_TREE_LIST:
    result = m_unitSize * m_value.expressions->length ();
    break;
  }

  ___CBTPOP;
  return result;
}

byteBlock_t::type_t data_t::type (void) {
  return TYPE_DATA;
}
