/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2012, 2014 Gerard Visser.

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

#ifndef ___TESTINPUT_INCLUDED
#define ___TESTINPUT_INCLUDED

/* The file `testinput' should contain utf8-encoding of the
   following ranges:
     0000 - 00FF
     0780 - 087F
     2000 - 20FF
     FF00 - FFFF */

#define TESTINPUT_NAME "testinput"
#define TESTINPUT_SIZE 0xA00

int testinput_nextByte (void);
int testinput_nextChar (void);
void testinput_reset (void);

#endif
