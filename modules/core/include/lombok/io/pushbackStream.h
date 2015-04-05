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

#ifndef ___LOMBOK__IO__PUSHBACK_STREAM_INCLUDED
#define ___LOMBOK__IO__PUSHBACK_STREAM_INCLUDED

#include <lombok/io/inputStream.h>

class pushbackStream_t : public inputStream_t {
private:
  inputStream_t* const stream;
  int buffer;

public:
  pushbackStream_t (inputStream_t* stream);
  virtual ~pushbackStream_t (void);

  virtual int      close (void);
  virtual long int getErrorCode (void);
  virtual bool     isByteStream (void);
  virtual int      read (void);
  virtual void     unread (int val);

  REFL_DECL
};

#endif
