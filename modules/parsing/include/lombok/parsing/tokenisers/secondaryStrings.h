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

#ifndef ___LOMBOK__PARSING__TOKENISERS__SECONDARY_STRINGS_INCLUDED
#define ___LOMBOK__PARSING__TOKENISERS__SECONDARY_STRINGS_INCLUDED

#include <lombok/util/string.h>

namespace pars {

  string_t* get21 (void);       // !
  string_t* get213D (void);     // !=
  string_t* get213D3D (void);   // !==
  string_t* get23 (void);       // #
  string_t* get2323 (void);     // ##
  string_t* get25 (void);       // %
  string_t* get253D (void);     // %=
  string_t* get26 (void);       // &
  string_t* get2626 (void);     // &&
  string_t* get263D (void);     // &=
  string_t* get28 (void);       // (
  string_t* get29 (void);       // )
  string_t* get2A (void);       // *
  string_t* get2A3D (void);     // *=
  string_t* get2B (void);       // +
  string_t* get2B2B (void);     // ++
  string_t* get2B3D (void);     // +=
  string_t* get2C (void);       // ,
  string_t* get2D (void);       // -
  string_t* get2D2D (void);     // --
  string_t* get2D3D (void);     // -=
  string_t* get2D3E (void);     // ->
  string_t* get2D3E2A (void);   // ->*
  string_t* get2E (void);       // .
  string_t* get2E2A (void);     // .*
  string_t* get2E2E (void);     // ..
  string_t* get2E2E2E (void);   // ...
  string_t* get2F (void);       // /
  string_t* get2F3D (void);     // /=
  string_t* get3A (void);       // :
  string_t* get3A3A (void);     // ::
  string_t* get3B (void);       // ;
  string_t* get3C (void);       // <
  string_t* get3C3C (void);     // <<
  string_t* get3C3C3D (void);   // <<=
  string_t* get3C3D (void);     // <=
  string_t* get3D (void);       // =
  string_t* get3D3D (void);     // ==
  string_t* get3D3D3D (void);   // ===
  string_t* get3E (void);       // >
  string_t* get3E3D (void);     // >=
  string_t* get3E3E (void);     // >>
  string_t* get3E3E3D (void);   // >>=
  string_t* get3E3E3E (void);   // >>>
  string_t* get3E3E3E3D (void); // >>>=
  string_t* get3F (void);       // ?
  string_t* get40 (void);       // @
  string_t* get5B (void);       // [
  string_t* get5C (void);       // \

  string_t* get5D (void);       // ]
  string_t* get5E (void);       // ^
  string_t* get5E3D (void);     // ^=
  string_t* get7B (void);       // {
  string_t* get7C (void);       // |
  string_t* get7C3D (void);     // |=
  string_t* get7C7C (void);     // ||
  string_t* get7D (void);       // }
  string_t* get7E (void);       // ~

}

#endif
