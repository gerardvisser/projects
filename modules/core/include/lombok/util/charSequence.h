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

#ifndef ___LOMBOK__UTIL__CHAR_SEQUENCE_INCLUDED
#define ___LOMBOK__UTIL__CHAR_SEQUENCE_INCLUDED

#include <lombok/base/class.h>
#include <lombok/base/uchar.h>

class charSequence_t : public object_t {
protected:
  uchar_t* buf;
  int _length;

public:
  virtual ~charSequence_t (void);

  virtual uchar_t      charAt (int index);
  virtual bool         equals (object_t* other);
  virtual unsigned int hashCode (void);
  virtual int          length (void);
  virtual char*        toUtf8 (char* dst = NULL, int allocSizeInBytes = 0, int* bytesWritten = NULL);

  static int compare (charSequence_t* x, charSequence_t* y);

  I_REFL_DECL

  friend class stringBuilder_t;
};

#endif
