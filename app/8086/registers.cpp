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

#include "registers.h"
#include <lombok/base/errors.h>

REFL_IMPL (registers_t, object_t)

registers_t::registers_t (void) {
  ___CBTPUSH;

  int i;
  for (i = 0; i < 4; ++i) {
    m_gen[i] = 0;
    m_seg[i] = 0x51;
  }
  for (i = 1; i < 4; ++i) {
    m_gen[i + 4] = 0;
  }
  m_gen[SP] = 0xFFFE;
  m_flags = 0xF202;
  m_ip = 0x100;

  ___CBTPOP;
}

int registers_t::ax (void) {
  return m_gen[AX];
}

int registers_t::cx (void) {
  return m_gen[CX];
}

int registers_t::dx (void) {
  return m_gen[DX];
}

int registers_t::bx (void) {
  return m_gen[BX];
}

int registers_t::sp (void) {
  return m_gen[SP];
}

int registers_t::bp (void) {
  return m_gen[BP];
}

int registers_t::si (void) {
  return m_gen[SI];
}

int registers_t::di (void) {
  return m_gen[DI];
}

int registers_t::gen (int index) {
  ___CBTPUSH;

  int result = m_gen[index];

  ___CBTPOP;
  return result;
}

int registers_t::es (void) {
  return m_seg[ES];
}

int registers_t::cs (void) {
  return m_seg[CS];
}

int registers_t::ss (void) {
  return m_seg[SS];
}

int registers_t::ds (void) {
  return m_seg[DS];
}

int registers_t::seg (int index) {
  ___CBTPUSH;

  int result = m_seg[index];

  ___CBTPOP;
  return result;
}

int registers_t::ip (void) {
  return m_ip;
}

int registers_t::flags (void) {
  return m_flags;
}

int registers_t::destinationIndex (void) {
  return ((int) m_seg[ES] << 4) + m_gen[DI];
}

int registers_t::instructionIndex (void) {
  return ((int) m_seg[CS] << 4) + m_ip;
}

int registers_t::sourceIndex (void) {
  return ((int) m_seg[DS] << 4) + m_gen[SI];
}

int registers_t::stackIndex (void) {
  return ((int) m_seg[SS] << 4) + m_gen[SP];
}
