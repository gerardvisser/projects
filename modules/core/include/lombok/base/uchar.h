/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013, 2014 Gerard Visser.

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

#ifndef ___LOMBOK__BASE__UCHAR_INCLUDED
#define ___LOMBOK__BASE__UCHAR_INCLUDED

typedef unsigned short int uchar_t;

int  uchar_hexDigitValue (uchar_t cr);
bool uchar_isBinDigit (uchar_t cr);
bool uchar_isDigit (uchar_t cr);
bool uchar_isHexDigit (uchar_t cr);
bool uchar_isOctDigit (uchar_t cr);
void uchar_put (uchar_t cr);
int  uchar_toUtf8 (uchar_t cr);

#endif
