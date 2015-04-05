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

#include "address.h"

REFL_IMPL (address_t, object_t)

address_t::address_t (unsigned short segment, unsigned short offset) {
  m_segment = segment;
  m_offset = offset;
}

void address_t::add (unsigned short val) {
  m_offset += val;
}

unsigned short address_t::offset (void) const {
  return m_offset;
}

unsigned short address_t::segment (void) const {
  return m_segment;
}

void address_t::setOffset (unsigned short val) {
  m_offset = val;
}

void address_t::setSegment (unsigned short val) {
  m_segment = val;
}

int address_t::value (void) const {
  return ((int) m_segment << 4) + m_offset;
}
