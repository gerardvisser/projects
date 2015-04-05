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

#ifndef ___DEBUG_REGISTERS_INCLUDED
#define ___DEBUG_REGISTERS_INCLUDED

#include <lombok/base/class.h>

class registers_t : public object_t {
private:
  unsigned short m_gen[8];  /* General Purpose Registers. */
  unsigned short m_seg[4];  /* Segment Registers. */
  unsigned short m_flags;   /* Flags Register. */
  unsigned short m_ip;      /* Instruction Pointer. */

public:
  registers_t (void);

  virtual int ax (void);
  virtual int cx (void);
  virtual int dx (void);
  virtual int bx (void);
  virtual int sp (void);
  virtual int bp (void);
  virtual int si (void);
  virtual int di (void);
  virtual int gen (int index);

  virtual int es (void);
  virtual int cs (void);
  virtual int ss (void);
  virtual int ds (void);
  virtual int seg (int index);

  virtual int ip (void);
  virtual int flags (void);

  virtual int destinationIndex (void);
  virtual int instructionIndex (void);
  virtual int sourceIndex (void);
  virtual int stackIndex (void);

  REFL_DECL

  friend class processor_t;
};

#define AX 0
#define CX 1
#define DX 2
#define BX 3
#define SP 4
#define BP 5
#define SI 6
#define DI 7

#define ES 0
#define CS 1
#define SS 2
#define DS 3

#define F_CARRY     0x0001
#define F_PARITY    0x0004
#define F_ADJUST    0x0010
#define F_ZERO      0x0040
#define F_SIGN      0x0080
#define F_TRAP      0x0100
#define F_INTERRUPT 0x0200
#define F_DIRECTION 0x0400
#define F_OVERFLOW  0x0800

#endif
