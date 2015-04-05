/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

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

#ifdef DEBUG_MODE

#include "dbgAllocInfo.h"

dbgAllocInfo_t::dbgAllocInfo_t (void) {
  allocCount = 0;
  freeCount = 0;
  allocated = 0;
  allocatedMax = 0;
  freed = 0;
}

dbgAllocInfo_t::~dbgAllocInfo_t (void) {
}

void dbgAllocInfo_t::add (long int sizeInBytes) {
  ++allocCount;
  allocated += sizeInBytes;
  updateMax ();
}

long int dbgAllocInfo_t::getAllocated (void) {
  return allocated;
}

long int dbgAllocInfo_t::getAllocatedMax (void) {
  return allocatedMax;
}

long int dbgAllocInfo_t::getAllocCount (void) {
  return allocCount;
}

long int dbgAllocInfo_t::getCurrentlyAllocated (void) {
  return allocated - freed;
}

long int dbgAllocInfo_t::getFreeCount (void) {
  return freeCount;
}

long int dbgAllocInfo_t::getFreed (void) {
  return freed;
}

void dbgAllocInfo_t::sub (long int sizeInBytes) {
  ++freeCount;
  freed += sizeInBytes;
}

void dbgAllocInfo_t::update (long int sizeInBytes) {
  if (sizeInBytes > 0) {
    allocated += sizeInBytes;
    updateMax ();
  } else {
    freed -= sizeInBytes;
  }
}

void dbgAllocInfo_t::updateMax (void) {
  long int allocatedCurrently = allocated - freed;
  if (allocatedCurrently > allocatedMax) {
    allocatedMax = allocatedCurrently;
  }
}

#endif
