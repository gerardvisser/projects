/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2014 Gerard Visser.

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

#ifndef ___GVASM__PROGRAMME_BUILDER_INCLUDED
#define ___GVASM__PROGRAMME_BUILDER_INCLUDED

#include "instructions/instructionFactory.h"
#include "programme.h"

class programmeBuilder_t : public object_t {
private:
  instructionFactory_t* m_instructionFactory;
  hashMap_t* m_labels;
  string_t* m_csLabel;

public:
  programmeBuilder_t (void);
  virtual ~programmeBuilder_t (void);

  virtual programme_t* build (arrayList_t* tokens);

private:
  int addByteBlockOrLabel (arrayList_t* byteBlocks, arrayList_t* orderedLabels, listIterator_t* tokens, pars::token_t* token, xValue_t* csOffset, int locationIndex);
  void determineLabelValues (arrayList_t* byteBlocks, arrayList_t* orderedLabels);
  void findLabels (arrayList_t* tokens, xValue_t* csOffset);

  REFL_DECL
};

#endif
