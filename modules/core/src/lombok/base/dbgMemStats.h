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

# ifndef ___LOMBOK__BASE__DBG_MEM_STATS_INCLUDED
#  define ___LOMBOK__BASE__DBG_MEM_STATS_INCLUDED

#  include "dbgAllocInfo.h"
#  include "dbgRefInfoHashList.h"

namespace dbgMemStats {
  dbgAllocInfo_t*       allocatedBytes (void);
  dbgAllocInfo_t*       debugBytes (void);
  dbgRefInfoHashList_t* list (void);
  void                  printExitStats (const char* fileName);
  dbgAllocInfo_t*       requestedBytes (void);
}

# endif

#endif
