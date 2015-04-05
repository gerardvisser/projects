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

#include <unistd.h>
#include <sys/stat.h>
#include <lombok/base/errors.h>
#include "filenameResolver.h"

REFL_IMPL (pars::filenameResolver_t, object_t)

using namespace pars;

filenameResolver_t::filenameResolver_t (void) {
  ___CBTPUSH;

  m_searchDirs = new arrayList_t (8);

  ___CBTPOP;
}

filenameResolver_t::~filenameResolver_t (void) {
  ___CBTPUSH;

  m_searchDirs->dump ();

  ___CBTPOP;
}

filename_t* filenameResolver_t::absoluteNameFor (filename_t* filename) {
  ___CBTPUSH;

  struct stat attrs;
  filename_t* result = NULL;
  if (!filename->isAbsolute ()) {
    int i = m_searchDirs->length () - 1;
    while (i > -1 && result == NULL) {
      result = ((filename_t*) m_searchDirs->get (i))->concat (filename);
      if (!(stat (result->get (), &attrs) == 0 && S_ISREG (attrs.st_mode))) {
        result->dump ();
        result = NULL;
      }
      --i;
    }
  } else if (stat (filename->get (), &attrs) == 0 && S_ISREG (attrs.st_mode)) {
    filename->reserve ();
    result = filename;
  }

  ___CBTPOP;
  return result;
}

void filenameResolver_t::addSearchDirectory (const char* dirname) {
  ___CBTPUSH;

  struct stat attrs;
  if (stat (dirname, &attrs) == 0) {
    if (S_ISDIR (attrs.st_mode) && access (dirname, R_OK | X_OK) == 0) {
      filename_t* dir = filename_t::create (dirname);
      if (dir->isAbsolute () && !m_searchDirs->contains (dir)) {
        m_searchDirs->append (dir);
      } else {
        dir->dump ();
      }
    }
  }

  ___CBTPOP;
}
