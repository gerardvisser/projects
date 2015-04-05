/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2008, 2009, 2010, 2011, 2012, 2013 Gerard Visser.

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

#ifdef DEBUG_MODE

#include <stdint.h>
#include <lombok/base/mem.h>
#include "dbgRefInfoHashList.h"

static const uint64_t M = 1073741827;
static const uint64_t A = 1048583;
static const uint64_t B = 440234149;

static int hashCode (const void* pointer) {
  uint64_t val = (uint64_t) (long) pointer;
  val &= 0x3FFFFFFF;
  val *= A;
  val += B;
  val %= M;
  return (int) val;
}

dbgRefInfoHashList_t::dbgRefInfoHashList_t (void) {
}

dbgRefInfoHashList_t::~dbgRefInfoHashList_t (void) {
}

void dbgRefInfoHashList_t::add (dbgRefInfo_t* elem) {
  int index = hashCode (elem->getPointer ()) % BUCKET_COUNT;
  bucket[index].add (elem);
}

void dbgRefInfoHashList_t::printContents (FILE* stream) {
  for (int i = 0; i < BUCKET_COUNT; ++i) {
    bucket[i].printContents (stream);
  }
}

dbgRefInfo_t* dbgRefInfoHashList_t::remove (const void* pointer) {
  int index = hashCode (pointer) % BUCKET_COUNT;
  bucket[index].remove (pointer);
}

#endif
