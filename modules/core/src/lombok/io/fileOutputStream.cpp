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

#ifndef IO_BUF_SIZE
# define IO_BUF_SIZE 0x1000
#endif

#include <errno.h>
#include <string.h>
#include <lombok/base/mem.h>
#include <lombok/base/errors.h>
#include <lombok/io/errorCodes.h>
#include <lombok/io/fileOutputStream.h>

REFL_IMPL (fileOutputStream_t, outputStream_t)

fileOutputStream_t::fileOutputStream_t (const char* fileName, bool append) {
  ___CBTPUSH;

  initialise (fileName, append);

  ___CBTPOP;
}

fileOutputStream_t::fileOutputStream_t (charSequence_t* fileName, bool append) {
  ___CBTPUSH;

  int bufSize = fileName->length () + 1;
  char buf[bufSize];
  char* utf8 = fileName->toUtf8 (buf, bufSize);
  initialise (utf8, append);
  if (utf8 != buf) {
    mem_release (utf8);
  }

  ___CBTPOP;
}

fileOutputStream_t::~fileOutputStream_t (void) {
  ___CBTPUSH;

  close ();

  ___CBTPOP;
}

int fileOutputStream_t::close (void) {
  ___CBTPUSH;

  int result;

  if (handle != NULL) {
    result = flush ();
    mem_release (buf);
    fclose (handle);
    handle = NULL;
  } else {
    result = 0;
  }

  ___CBTPOP;
  return result;
}

void fileOutputStream_t::determineErrorCode (void) {
  ___CBTPUSH;

  switch (errno) {
  case EACCES:
    errorCode = IO_ERR__NO_WRITE_PERMISSION;
    break;
  case EIO:
    errorCode = IO_ERR__HARDWARE_ERROR;
    break;
  case EISDIR:
    errorCode = IO_ERR__IS_DIRECTORY;
    break;
  case EMFILE:
    errorCode = IO_ERR__TOO_MANY_STREAMS_OPEN;
    break;
  case ENOSPC:
    errorCode = IO_ERR__NO_DISK_SPACE_LEFT;
    break;
  default:
    errorCode = IO_ERR__UNKNOWN_ERROR;
    break;
  }

  ___CBTPOP;
}

int fileOutputStream_t::flush (void) {
  ___CBTPUSH;

  int result;
  if (index > 0) {
    result = writeBuffer ();
  } else {
    result = 0;
  }
  if (result == 0  &&  fflush (handle) == EOF) {
    determineErrorCode ();
    result = -1;
  }

  ___CBTPOP;
  return result;
}

void fileOutputStream_t::initialise (const char* fileName, bool append) {
  ___CBTPUSH;

  if (append) {
    handle = fopen (fileName, "ab");
  } else {
    handle = fopen (fileName, "wb");
  }
  if (handle == NULL) {
    determineErrorCode ();
    buf = NULL;
  } else {
    errorCode = 0;
    buf = (char*) mem_allocate ("char[]", IO_BUF_SIZE, PTYPE_INTERNAL);
  }
  index = 0;

  ___CBTPOP;
}

bool fileOutputStream_t::isByteStream (void) {
  return true;
}

int fileOutputStream_t::write (int val) {
  ___CBTPUSH;

  int result;
  if (index == IO_BUF_SIZE) {
    result = writeBuffer ();
  } else {
    result = 0;
  }
  if (index < IO_BUF_SIZE) {
    buf[index] = (char) val;
    ++index;
  }

  ___CBTPOP;
  return result;
}

int fileOutputStream_t::write (const char* bytes, int size) {
  ___CBTPUSH;

  int result;
  if (index == IO_BUF_SIZE) {
    result = writeBuffer ();
  } else {
    result = 0;
  }
  if (result == 0) {
    if (size < 0) {
      size = strlen (bytes);
    }
    if (size > IO_BUF_SIZE - index) {
      result = writeLargeBlock (bytes, size);
    } else {
      memcpy (buf + index, bytes, size);
      index += size;
    }
  }

  ___CBTPOP;
  return result;
}

int fileOutputStream_t::write (charSequence_t* str) {
  ___CBTPUSH;

  int result;
  if (index == IO_BUF_SIZE) {
    result = writeBuffer ();
  } else {
    result = 0;
  }
  if (result == 0) {
    int bytesWritten;
    char* buffer = buf + index;
    char* outputBuffer = str->toUtf8 (buffer, IO_BUF_SIZE - index, &bytesWritten);
    if (outputBuffer != buffer) {
      result = writeLargeBlock (outputBuffer, bytesWritten);
      mem_release (outputBuffer);
    } else {
      index += bytesWritten;
    }
  }

  ___CBTPOP;
  return result;
}

int fileOutputStream_t::writeBuffer (void) {
  ___CBTPUSH;

  int result;
  int numberOfBytesWritten = fwrite (buf, 1, index, handle);
  if (numberOfBytesWritten < index) {
    if (numberOfBytesWritten > 0) {
      index -= numberOfBytesWritten;
      memmove (buf, buf + numberOfBytesWritten, index);
    }
    determineErrorCode ();
    result = -1;
  } else {
    index = 0;
    result = 0;
  }

  ___CBTPOP;
  return result;
}

/* size > IO_BUF_SIZE - this->index */
int fileOutputStream_t::writeLargeBlock (const char* bytes, int size) {
  ___CBTPUSH;

  int q = IO_BUF_SIZE - index;
  memcpy (buf + index, bytes, q);
  index = IO_BUF_SIZE;
  int result = writeBuffer ();
  if (result == 0) {
    bytes += q;
    size -= q;
    q = size / IO_BUF_SIZE;
    if (q > 0) {
      q *= IO_BUF_SIZE;
      if (fwrite (bytes, 1, q, handle) < q) {
        determineErrorCode ();
        result = -1;
      }
      bytes += q;
      size -= q;
    }
    memcpy (buf, bytes, size);
    index = size;
  }

  ___CBTPOP;
  return result;
}
