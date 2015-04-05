/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013, 2014 Gerard Visser.

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

#ifndef ___LOMBOK__UTIL__STRING_BUILDER_INCLUDED
#define ___LOMBOK__UTIL__STRING_BUILDER_INCLUDED

#include <lombok/util/string.h>

class stringBuilder_t : public charSequence_t {
private:
  int m_capacityQuantum;
  int m_capacity;

public:
  stringBuilder_t (int initialCapacity = 32);

  virtual void      append (uchar_t val);
  virtual void      append (uchar_t val, int numberOfTimes);
  virtual void      append (charSequence_t* val);
  virtual void      clear (void);
  virtual void      removeLastCharacter (void);
  virtual string_t* toString (void);

private:
  void ensureCapacity (int additionalLength);

  REFL_DECL
};

#endif
