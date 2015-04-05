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

#ifndef ___GVASM__ASM_WRITER_INCLUDED
#define ___GVASM__ASM_WRITER_INCLUDED

#include <lombok/io/outputStream.h>

class asmWriter_t : public object_t {
private:
  outputStream_t* m_stream;

public:
  asmWriter_t (outputStream_t* byteStream);
  virtual ~asmWriter_t (void);

  virtual void write (int val, bool wide);
  virtual void writeByte (int val);
  virtual void writeWord (int val);

  REFL_DECL
};

#endif
