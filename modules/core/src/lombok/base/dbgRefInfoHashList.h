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

# ifndef ___LOMBOK__BASE__DBG_REF_INFO_HASH_LIST_INCLUDED
#  define ___LOMBOK__BASE__DBG_REF_INFO_HASH_LIST_INCLUDED

#  include "dbgRefInfoList.h"

#  define BUCKET_COUNT 0x800

class dbgRefInfoHashList_t {
private:
  dbgRefInfoList_t bucket[BUCKET_COUNT];

public:
  dbgRefInfoHashList_t (void);
  virtual ~dbgRefInfoHashList_t (void);

  void          add (dbgRefInfo_t* elem);
  void          printContents (FILE* stream = stdout);
  dbgRefInfo_t* remove (const void* pointer);
};

# endif

#endif
