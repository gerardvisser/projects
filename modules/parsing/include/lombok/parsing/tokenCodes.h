/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013 Gerard Visser.

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

#ifndef ___LOMBOK__PARSING__TOKEN_CODES_INCLUDED
#define ___LOMBOK__PARSING__TOKEN_CODES_INCLUDED

#define TCODE_ERROR 3
#define TCODE_WARN  2

#define TCODE_COMMENT   (1 << 2)
#define TCODE_FUNC      (2 << 2)
#define TCODE_KEYWORD   (3 << 2)
#define TCODE_NAMESPACE (4 << 2)
#define TCODE_PRE       (5 << 2)
#define TCODE_STRING    (6 << 2)
#define TCODE_TYPE      (7 << 2)
#define TCODE_VAR       (8 << 2)

#endif
