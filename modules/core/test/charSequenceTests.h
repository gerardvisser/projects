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

#ifndef ___CHAR_SEQUENCE_TESTS_INCLUDED
#define ___CHAR_SEQUENCE_TESTS_INCLUDED

#include <lombok/util/charSequence.h>

namespace charsequenceTests {
  int length (const uchar_t* str);

  void testCharAt (charSequence_t* (* newCharSequence)(const uchar_t* str));
  void testEquals (charSequence_t* (* newCharSequence)(const uchar_t* str));
  void testHashCode (charSequence_t* (* newCharSequence)(const uchar_t* str));
  void testLength (charSequence_t* (* newCharSequence)(const uchar_t* str));
  void testToUtf8 (charSequence_t* (* newCharSequence)(const uchar_t* str));
}

/* charSequenceTests should be run after stringTests and stringBuilderTests. */

struct charSequenceTestsStruct {
  void (* testCompare)(void);
};

extern const struct charSequenceTestsStruct charSequenceTests;

#endif
