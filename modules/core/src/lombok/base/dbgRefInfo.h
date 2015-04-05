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

# ifndef ___LOMBOK__BASE__DBG_REF_INFO_INCLUDED
#  define ___LOMBOK__BASE__DBG_REF_INFO_INCLUDED

#  define PTYPE_VIRTUAL             8 /* See also lombok/base/mem.h */

class dbgRefInfo_t {
private:
  dbgRefInfo_t* previous;
  dbgRefInfo_t* next;

  const void* pointer;
  long int size;
  const char* const typeName;
  const int ptype;

public:
  dbgRefInfo_t (const void* pointer, const char* typeName, long int size, int ptype);
  virtual ~dbgRefInfo_t (void);

  dbgRefInfo_t* getNext (void);
  const void*   getPointer (void);
  dbgRefInfo_t* getPrevious (void);
  int           getPtype (void);
  long int      getRefCount (void);
  long int      getSize (void);
  const char*   getTypeName (void);
  void          setNext (dbgRefInfo_t* val);
  void          setPointer (const void* val);
  void          setPrevious (dbgRefInfo_t* val);
  void          setSize (long int val);
};

# endif

#endif
