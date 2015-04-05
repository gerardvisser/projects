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

#ifndef ___LOMBOK__BASE__AUX_LIST_INCLUDED
#define ___LOMBOK__BASE__AUX_LIST_INCLUDED

#include <lombok/base/class.h>

typedef struct auxListElemStruct auxListElem_t;

struct auxListElemStruct {
  auxListElem_t* previous;
  auxListElem_t* next;
};

class auxList_t : public object_t {
private:
  auxListElem_t* head;
  auxListElem_t* tail;
  void (* deleteElem)(auxListElem_t* elem);

public:
  auxList_t (void (* deleteElem)(auxListElem_t* elem));
  virtual ~auxList_t (void);

  void           appendToHead (auxListElem_t* elem);
  void           appendToTail (auxListElem_t* elem);
  auxListElem_t* getHead (void);
  void           remove (auxListElem_t* elem);
  void           removeAndDelete (auxListElem_t* elem);

  REFL_DECL

  static void* operator new (size_t s);
};

#endif
