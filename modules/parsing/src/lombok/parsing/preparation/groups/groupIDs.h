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

#ifndef ___LOMBOK__PARSING__PREPARATION__GROUPS__GROUP_I_DS_INCLUDED
#define ___LOMBOK__PARSING__PREPARATION__GROUPS__GROUP_I_DS_INCLUDED

#include <lombok/parsing/blockTypes.h>

#define GID_DEFINE       (1 << WIDTH_BLTYPE | BLTYPE_GROUP)
#define GID_ELIF         (2 << WIDTH_BLTYPE | BLTYPE_GROUP)
#define GID_ELSE         (3 << WIDTH_BLTYPE | BLTYPE_GROUP)
#define GID_ENDIF        (4 << WIDTH_BLTYPE | BLTYPE_GROUP)
#define GID_IF           (5 << WIDTH_BLTYPE | BLTYPE_GROUP)
#define GID_IFDEF        (6 << WIDTH_BLTYPE | BLTYPE_GROUP)
#define GID_IFNDEF       (7 << WIDTH_BLTYPE | BLTYPE_GROUP)
#define GID_INCLUDE      (8 << WIDTH_BLTYPE | BLTYPE_GROUP)
#define GID_PARAM        (9 << WIDTH_BLTYPE | BLTYPE_GROUP)
#define GID_UNDEF        (10 << WIDTH_BLTYPE | BLTYPE_GROUP)

#endif
