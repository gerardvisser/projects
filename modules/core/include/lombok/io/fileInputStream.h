/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2012, 2013, 2014 Gerard Visser.

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

#ifndef ___LOMBOK__IO__FILE_INPUT_STREAM_INCLUDED
#define ___LOMBOK__IO__FILE_INPUT_STREAM_INCLUDED

#include <stdio.h>
#include <lombok/io/inputStream.h>
#include <lombok/util/charSequence.h>

class fileInputStream_t : public inputStream_t {
private:
  FILE* m_handle;
  unsigned char* m_buf;
  int m_index;
  int m_size;

public:
  fileInputStream_t (const char* fileName);
  fileInputStream_t (charSequence_t* fileName);
  virtual ~fileInputStream_t (void);

  virtual int  close (void);
  virtual bool isByteStream (void);
  virtual int  read (void);
  virtual int  read (unsigned char* dst, int count);

private:
  void initialise (const char* fileName);

  REFL_DECL
};

#endif
