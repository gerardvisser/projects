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

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <lombok/base/errors.h>
#include <lombok/util/string.h>
#include <lombok/util/stringBuilder.h>
#include "asmErrors.h"

static const char* filename = NULL;

void asmErrors::exit (const char* errorMessage) {
  ___BTPUSH;

  std::fprintf (stderr, "error: %s.\nCompilation terminated.\n", errorMessage);
  std::exit (EXIT_FAILURE);

  ___BTPOP;
}

void asmErrors::exit (const char* errorMessage, int line, int column) {
  ___BTPUSH;

  std::fprintf (stderr, "%s:%d:%d: ", filename, line, column);
  asmErrors::exit (errorMessage);

  ___BTPOP;
}

void asmErrors::exit (const char* errorMessage, pars::token_t* token, ...) {
  ___BTPUSH;

  int i = 0;
  va_list argp;
  va_start (argp, token);
  stringBuilder_t* message = new stringBuilder_t (256);
  while (errorMessage[i] != 0) {
    while (!(errorMessage[i] == '%' || errorMessage[i] == 0)) {
      switch (errorMessage[i]) {
      case '\x91': message->append (0x2018); break;
      case '\x92': message->append (0x2019); break;
      default: message->append ((unsigned char) errorMessage[i]);
      }
      ++i;
    }
    if (errorMessage[i] == '%') {
      ++i;
      switch (errorMessage[i]) {
      case 'c': {
        int arg = va_arg (argp, int);
        message->append (arg);
      } break;

      case 'd': {
        char val[16];
        int arg = va_arg (argp, int);
        std::sprintf (val, "%d", arg);
        string_t* str = new string_t (val);
        message->append (str);
        str->dump ();
      } break;

      case 's': {
        char* arg = va_arg (argp, char*);
        string_t* str = new string_t (arg);
        message->append (str);
        str->dump ();
      } break;

      case 't':
        message->append (token->text ());
        break;

      default: --i;
      }
      ++i;
    }
  }
  va_end (argp);
  exit (message->toUtf8 (), token->beginLine (), token->beginColumn ());

  ___BTPOP;
}

void asmErrors::init (const char* _filename) {
  ___BTPUSH;

  filename = _filename;

  ___BTPOP;
}
