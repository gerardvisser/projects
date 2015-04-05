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

#ifndef ___LOMBOK__UTIL__STRING_INCLUDED
#define ___LOMBOK__UTIL__STRING_INCLUDED

#include <lombok/util/charSequence.h>

#define CHAR_ENC_ISO8859_1 1
#define CHAR_ENC_ISO8859_2 2
#define CHAR_ENC_UTF8      3

class string_t : public charSequence_t {
private:
  unsigned int _hashCode;

public:
  string_t (const char* str, int characterEncoding = CHAR_ENC_UTF8);
  string_t (const uchar_t* str, int len);

  virtual unsigned int hashCode (void);
  virtual string_t*    substring (int startIndex, int length);

  REFL_DECL
};

#endif
