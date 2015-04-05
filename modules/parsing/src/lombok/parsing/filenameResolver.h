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

#ifndef ___LOMBOK__PARSING__FILENAME_RESOLVER_INCLUDED
#define ___LOMBOK__PARSING__FILENAME_RESOLVER_INCLUDED

#include <lombok/io/filename.h>
#include <lombok/util/arrayList.h>

namespace pars {

  class filenameResolver_t : public object_t {
  private:
    arrayList_t* m_searchDirs;

  public:
    filenameResolver_t (void);
    virtual ~filenameResolver_t (void);

    /**
     * Returns null if no file with the specified name can be found in
     * any of the search directories. If two or more directories contain
     * such a file, the first one found is returned. The caller owns the
     * objects returned by this method.
     */
    virtual filename_t* absoluteNameFor (filename_t* filename);

    /**
     * Adds a directory to search; the last one added will be searched first.
     * The specified names should be absolute.
     */
    virtual void addSearchDirectory (const char* dirname);

    REFL_DECL
  };

}

#endif
