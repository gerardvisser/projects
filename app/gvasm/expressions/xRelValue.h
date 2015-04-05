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

#ifndef ___GVASM__EXPRESSIONS__X_REL_VALUE_INCLUDED
#define ___GVASM__EXPRESSIONS__X_REL_VALUE_INCLUDED

#include "xValue.h"

class xRelValue_t : public xValue_t {
private:
  xValue_t* const m_base;

public:
  xRelValue_t (xValue_t* base, int64_t value = 0);
  virtual ~xRelValue_t (void);

  virtual int64_t get (void);

  REFL_DECL
};

#endif
