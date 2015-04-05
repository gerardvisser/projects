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

#include <lombok/base/errors.h>
#include "asmErrors.h"
#include "asmWriter.h"

REFL_IMPL (asmWriter_t, object_t)

asmWriter_t::asmWriter_t (outputStream_t* byteStream) : m_stream (byteStream) {
  ___CBTPUSH;
  ___CBTPOP;
}

asmWriter_t::~asmWriter_t (void) {
  ___CBTPUSH;

  m_stream->dump ();

  ___CBTPOP;
}

void asmWriter_t::write (int val, bool wide) {
  ___CBTPUSH;

  if (wide) {
    writeWord (val);
  } else {
    writeByte (val);
  }

  ___CBTPOP;
}

void asmWriter_t::writeByte (int val) {
  ___CBTPUSH;

  m_stream->write (val);
  if (m_stream->getErrorCode () != 0) {
    asmErrors::exit (m_stream->getErrorDescription ());
  }

  ___CBTPOP;
}

void asmWriter_t::writeWord (int val) {
  ___CBTPUSH;

  writeByte (val);
  writeByte (val >> 8);

  ___CBTPOP;
}
