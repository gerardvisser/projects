/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2012, 2014 Gerard Visser.

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

#include <errno.h>
#include <sys/stat.h>
#include <lombok/base/errors.h>

static const char* xsperms_errorMessage (void) {
  const char* result;
  switch (errno) {
  case EACCES:
    result = "The process does not have search permission for a directory component of the filename (%s)";
    break;
  case ENOENT:
    result = "File `%s' does not exist";
    break;
  case EPERM:
    result = "This process does not have permission to change the access permissions of file `%s'";
    break;
  default: /* ELOOP, ENAMETOOLONG, ENOTDIR, EROFS */
    result = "An unexpected error occurred";
    break;
  }
  return result;
}

void xsperms_chmod (const char* fileName, int mode) {
  ___BTPUSH;
  if (chmod (fileName, mode)) {
    errors_printMessageAndExit (xsperms_errorMessage (), fileName);
  }
  ___BTPOP;
}
