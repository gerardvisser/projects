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

#ifndef ___LOMBOK__PARSING__BLOCK_TYPES_INCLUDED
#define ___LOMBOK__PARSING__BLOCK_TYPES_INCLUDED

#define MASK_BLTYPE      ((1 << WIDTH_BLTYPE) - 1)
#define WIDTH_BLTYPE     3

#define BLTYPE_CHAR      1
#define BLTYPE_COMMENT   2
#define BLTYPE_GROUP     3
#define BLTYPE_NUMBER    4
#define BLTYPE_PRIMARY   5
#define BLTYPE_SECONDARY 6
#define BLTYPE_STRING    7

#endif
