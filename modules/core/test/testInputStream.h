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

#ifndef ___TEST_INPUT_STREAM_INCLUDED
#define ___TEST_INPUT_STREAM_INCLUDED

#include <lombok/io/inputStream.h>

class testInputStream_t : public inputStream_t {
private:
  const int m_maxToRead;
  int m_nextToRead;
  bool m_closeWasCalled;
  bool m_byteStream;

public:
  testInputStream_t (int maxToRead = 10);

  virtual int  close (void);
  virtual bool closeWasCalled (void);
  virtual bool isByteStream (void);
  virtual int  read (void);
  virtual void setByteStream (bool val);
  virtual void setErrorCode (long int val);

  REFL_DECL
};

#endif
