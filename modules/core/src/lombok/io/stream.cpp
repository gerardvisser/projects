/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2012, 2013 Gerard Visser.

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

#include <lombok/io/stream.h>
#include <lombok/base/errors.h>
#include <lombok/io/errorCodes.h>

I_REFL_IMPL (stream_t, object_t)

long int stream_t::getErrorCode (void) {
  return errorCode;
}

const char* stream_t::getErrorDescription (void) {
  ___CBTPUSH;

  const char* description;
  switch (getErrorCode ()) {
  case IO_ERR__HARDWARE_ERROR:
    description = "Hardware error.";
    break;
  case IO_ERR__IS_DIRECTORY:
    description = "The specified file is a directory.";
    break;
  case IO_ERR__NO_DISK_SPACE_LEFT:
    description = "No disk space left.";
    break;
  case IO_ERR__NO_READ_PERMISSION:
    description = "No permission to read.";
    break;
  case IO_ERR__NO_SUCH_FILE:
    description = "No such file.";
    break;
  case IO_ERR__NO_WRITE_PERMISSION:
    description = "No permission to write.";
    break;
  case IO_ERR__TOO_MANY_STREAMS_OPEN:
    description = "Too many streams open.";
    break;
  case IO_ERR__UNKNOWN_ERROR:
    description = "Unknown error.";
    break;
  default:
    description = NULL;
  }

  ___CBTPOP;
  return description;
}
