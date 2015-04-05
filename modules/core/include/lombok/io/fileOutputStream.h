/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2012, 2013 Gerard Visser.

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

#ifndef ___LOMBOK__IO__FILE_OUTPUT_STREAM_INCLUDED
#define ___LOMBOK__IO__FILE_OUTPUT_STREAM_INCLUDED

#include <stdio.h>
#include <lombok/io/outputStream.h>

class fileOutputStream_t : public outputStream_t {
private:
  FILE* handle;
  char* buf;
  int index;

public:
  fileOutputStream_t (const char* fileName, bool append = false);
  fileOutputStream_t (charSequence_t* fileName, bool append = false);
  virtual ~fileOutputStream_t (void);

  virtual int  close (void);
  virtual int  flush (void);
  virtual bool isByteStream (void);
  virtual int  write (int val);
  virtual int  write (const char* bytes, int size = -1);
  virtual int  write (charSequence_t* str);

private:
  void determineErrorCode (void);
  void initialise (const char* fileName, bool append);
  int  writeBuffer (void);
  int  writeLargeBlock (const char* bytes, int size);

  REFL_DECL
};

#endif
