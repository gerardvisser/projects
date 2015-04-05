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

#include <lombok/base/errors.h>
#include <lombok/base/objectReleasePool.h>
#include <lombok/parsing/tokenisers/secondaryStrings.h>

#define GET_IMPL(elemnumber, text) \
  static string_t* elem ## elemnumber = NULL; \
  string_t* pars::get ## elemnumber (void) { \
    ___BTPUSH; \
    if (elem ## elemnumber == NULL) { \
      elem ## elemnumber = new string_t (text); \
      objectReleasePool_t::instance ()->add (elem ## elemnumber); \
    } \
    elem ## elemnumber->reserve (); \
    ___BTPOP; \
    return elem ## elemnumber; \
  }

GET_IMPL (21, "!")
GET_IMPL (213D, "!=")
GET_IMPL (213D3D, "!==")
GET_IMPL (23, "#")
GET_IMPL (2323, "##")
GET_IMPL (25, "%")
GET_IMPL (253D, "%=")
GET_IMPL (26, "&")
GET_IMPL (2626, "&&")
GET_IMPL (263D, "&=")
GET_IMPL (28, "(")
GET_IMPL (29, ")")
GET_IMPL (2A, "*")
GET_IMPL (2A3D, "*=")
GET_IMPL (2B, "+")
GET_IMPL (2B2B, "++")
GET_IMPL (2B3D, "+=")
GET_IMPL (2C, ",")
GET_IMPL (2D, "-")
GET_IMPL (2D2D, "--")
GET_IMPL (2D3D, "-=")
GET_IMPL (2D3E, "->")
GET_IMPL (2D3E2A, "->*")
GET_IMPL (2E, ".")
GET_IMPL (2E2A, ".*")
GET_IMPL (2E2E, "..")
GET_IMPL (2E2E2E, "...")
GET_IMPL (2F, "/")
GET_IMPL (2F3D, "/=")
GET_IMPL (3A, ":")
GET_IMPL (3A3A, "::")
GET_IMPL (3B, ";")
GET_IMPL (3C, "<")
GET_IMPL (3C3C, "<<")
GET_IMPL (3C3C3D, "<<=")
GET_IMPL (3C3D, "<=")
GET_IMPL (3D, "=")
GET_IMPL (3D3D, "==")
GET_IMPL (3D3D3D, "===")
GET_IMPL (3E, ">")
GET_IMPL (3E3D, ">=")
GET_IMPL (3E3E, ">>")
GET_IMPL (3E3E3D, ">>=")
GET_IMPL (3E3E3E, ">>>")
GET_IMPL (3E3E3E3D, ">>>=")
GET_IMPL (3F, "?")
GET_IMPL (40, "@")
GET_IMPL (5B, "[")
GET_IMPL (5C, "\\")
GET_IMPL (5D, "]")
GET_IMPL (5E, "^")
GET_IMPL (5E3D, "^=")
GET_IMPL (7B, "{")
GET_IMPL (7C, "|")
GET_IMPL (7C3D, "|=")
GET_IMPL (7C7C, "||")
GET_IMPL (7D, "}")
GET_IMPL (7E, "~")
