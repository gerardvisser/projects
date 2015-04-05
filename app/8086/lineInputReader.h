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

#ifndef ___DEBUG_LINE_INPUT_READER_INCLUDED
#define ___DEBUG_LINE_INPUT_READER_INCLUDED

#include <lombok/base/class.h>

class lineInputReader_t : public object_t {
private:
  char* m_buffer;
  size_t m_size;
  int m_position;
  int m_charsRead;
  int m_startPosition;

public:
  lineInputReader_t (void);
  virtual ~lineInputReader_t (void);

  virtual void getLineFromStdin (void);
  virtual int position (void);
  virtual int read (bool convertToUpper = true);
  virtual char* readAll (char* dynamicallyAllocatedBuffer, int* bufferSize, bool convertToUpper = true);
  virtual int readNextNonspace (bool convertToUpper = true);
  virtual void rewind (int count = 1);
  virtual void rewindEntirely (void);
  virtual void skip (int count);

  REFL_DECL
};

#endif
