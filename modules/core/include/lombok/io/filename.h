/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013, 2014 Gerard Visser.

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

#ifndef ___LOMBOK__IO__FILENAME_INCLUDED
#define ___LOMBOK__IO__FILENAME_INCLUDED

#include <lombok/util/charSequence.h>

class filename_t : public object_t {
#ifdef DEBUG_MODE
public:
#else
private:
#endif
  const char* const m_name;
  const int* const m_indices;
  const int m_indexCount;
  const int m_parentDirs;

public:
  virtual ~filename_t (void);

  virtual filename_t*  concat (filename_t* other);
  virtual bool         equals (object_t* other);
  virtual const char*  get (void);
  virtual unsigned int hashCode (void);
  virtual bool         isAbsolute (void);
  virtual bool         isReflexive (void);
  virtual bool         isRoot (void);
  virtual filename_t*  parentDirectory (void);
  virtual filename_t*  withExtension (const char* extension);

  static filename_t* create (const char* filename);
  static filename_t* create (charSequence_t* filename);
  static filename_t* workingDirectory (void);

private:
  filename_t (const char* name, const int* indices, int indexCount, int parentDirs);

  REFL_DECL
};

#endif
