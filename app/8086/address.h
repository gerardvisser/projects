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

#ifndef ___DEBUG_ADDRESS_INCLUDED
#define ___DEBUG_ADDRESS_INCLUDED

#include <lombok/base/class.h>

class address_t : public object_t {
private:
  unsigned short m_segment;
  unsigned short m_offset;

public:
  address_t (unsigned short segment = 0, unsigned short offset = 0);

  virtual void add (unsigned short val);
  virtual unsigned short offset (void) const;
  virtual unsigned short segment (void) const;
  virtual void setOffset (unsigned short val);
  virtual void setSegment (unsigned short val);
  virtual int value (void) const;

  REFL_DECL
};

#endif
