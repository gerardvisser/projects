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

#ifndef ___LOMBOK__PARSING__TOKEN_I_DS__C_TOKEN_I_DS_INCLUDED
#define ___LOMBOK__PARSING__TOKEN_I_DS__C_TOKEN_I_DS_INCLUDED

#include <lombok/parsing/tokenIDs/tokenIDs.h>

/* Primary tokens: */
#define WIDTH_TID_PI   6 /* To make them unique. */
#define WIDTH_TID_PDIR 1 /* Preprocessor directive or not. */
#define WIDTH_TID_PCAT 3 /* Category for C. */

#define MASK_TID_PDIR   (MASK_BLTYPE | ((1 << WIDTH_TID_PDIR) - 1) << WIDTH_BLTYPE + WIDTH_TID_PI)
#define MASK_TID_PCAT (MASK_BLTYPE | ((1 << WIDTH_TID_PCAT) - 1) << WIDTH_BLTYPE + WIDTH_TID_PI + WIDTH_TID_PDIR)

#define TID_DIRECTIVE (1 << WIDTH_BLTYPE + WIDTH_TID_PI | BLTYPE_PRIMARY)

#define TID_CAT_TYPE          (1 << WIDTH_BLTYPE + WIDTH_TID_PI + WIDTH_TID_PDIR | BLTYPE_PRIMARY)
#define TID_CAT_SUE           (2 << WIDTH_BLTYPE + WIDTH_TID_PI + WIDTH_TID_PDIR | BLTYPE_PRIMARY)
#define TID_CAT_STATEMENT     (3 << WIDTH_BLTYPE + WIDTH_TID_PI + WIDTH_TID_PDIR | BLTYPE_PRIMARY)
#define TID_CAT_STORAGECLASS  (4 << WIDTH_BLTYPE + WIDTH_TID_PI + WIDTH_TID_PDIR | BLTYPE_PRIMARY)
#define TID_CAT_TYPEQUALIFIER (5 << WIDTH_BLTYPE + WIDTH_TID_PI + WIDTH_TID_PDIR | BLTYPE_PRIMARY)

#define TID_P_AUTO         (1 << WIDTH_BLTYPE | TID_CAT_STORAGECLASS)
#define TID_P_BOOL         (2 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_BREAK        (3 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_CASE         (4 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_CHAR         (5 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_CONST        (6 << WIDTH_BLTYPE | TID_CAT_TYPEQUALIFIER)
#define TID_P_CONTINUE     (7 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_DEFAULT      (8 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_DEFINE       (9 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_DEFINED      (10 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_DO           (11 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_DOUBLE       (12 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_ELIF         (13 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_ELSE         (14 << WIDTH_BLTYPE | TID_DIRECTIVE | TID_CAT_STATEMENT)
#define TID_P_ENDIF        (15 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_ENUM         (16 << WIDTH_BLTYPE | TID_CAT_SUE)
#define TID_P_ERROR        (17 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_EXTERN       (18 << WIDTH_BLTYPE | TID_CAT_STORAGECLASS)
#define TID_P_FALSE        (19 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_FLOAT        (20 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_FOR          (21 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_FORTRAN      (22 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_GOTO         (23 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_IF           (24 << WIDTH_BLTYPE | TID_DIRECTIVE | TID_CAT_STATEMENT)
#define TID_P_IFDEF        (25 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_IFNDEF       (26 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_INCLUDE      (27 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_INLINE       (28 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_INT          (29 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_LINE         (30 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_LONG         (31 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_NULL         (32 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_PRAGMA       (33 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_REGISTER     (34 << WIDTH_BLTYPE | TID_CAT_STORAGECLASS)
#define TID_P_RESTRICT     (35 << WIDTH_BLTYPE | TID_CAT_TYPEQUALIFIER)
#define TID_P_RETURN       (36 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_SHORT        (37 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_SIGNED       (38 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_SIZEOF       (39 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_STATIC       (40 << WIDTH_BLTYPE | TID_CAT_STORAGECLASS)
#define TID_P_STRUCT       (41 << WIDTH_BLTYPE | TID_CAT_SUE)
#define TID_P_SWITCH       (42 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_TRUE         (43 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_TYPEDEF      (44 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_UNDEF        (45 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_UNION        (46 << WIDTH_BLTYPE | TID_CAT_SUE)
#define TID_P_UNSIGNED     (47 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_VOID         (48 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_VOLATILE     (49 << WIDTH_BLTYPE | TID_CAT_TYPEQUALIFIER)
#define TID_P_WARNING      (50 << WIDTH_BLTYPE | TID_DIRECTIVE)
#define TID_P_WHILE        (51 << WIDTH_BLTYPE | TID_CAT_STATEMENT)

#endif
