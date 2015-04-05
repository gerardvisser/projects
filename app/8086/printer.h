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

#ifndef ___DEBUG_PRINTER_INCLUDED
#define ___DEBUG_PRINTER_INCLUDED

#include "address.h"
#include "memory.h"
#include "registers.h"

class printer_t : public object_t {
private:
  registers_t* registers;
  memory_t* memory;

public:
  printer_t (registers_t* registers, memory_t* memory);
  virtual ~printer_t (void);

  virtual void printCode (address_t* address, int maxBytes = 0x20);
  virtual void printMemory (address_t* address, int maxBytes = 0x80);
  virtual void printRegisters (void);

private:
  int printInstruction (address_t* address);
  int printMemLine (address_t* address, int maxBytes);

  REFL_DECL
};

#endif
