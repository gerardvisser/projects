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

#ifndef ___LOMBOK__PARSING__TOKEN_I_DS__JAVA_TOKEN_I_DS_INCLUDED
#define ___LOMBOK__PARSING__TOKEN_I_DS__JAVA_TOKEN_I_DS_INCLUDED

#include <lombok/parsing/tokenIDs/tokenIDs.h>

/* Primary tokens: */
#define WIDTH_TID_PI   6 /* To make them unique. */
#define WIDTH_TID_PCAT 3 /* Category for Java. */

#define MASK_TID_PCAT (MASK_BLTYPE | ((1 << WIDTH_TID_PCAT) - 1) << WIDTH_BLTYPE + WIDTH_TID_PI)

/* TODO: THESE ARE C-CATEGORIES. THEY SHOULD BE MADE JAVA-SPECIFIC. */

#define TID_CAT_TYPE          (1 << WIDTH_BLTYPE + WIDTH_TID_PI | BLTYPE_PRIMARY)
#define TID_CAT_SUE           (2 << WIDTH_BLTYPE + WIDTH_TID_PI | BLTYPE_PRIMARY)
#define TID_CAT_STATEMENT     (3 << WIDTH_BLTYPE + WIDTH_TID_PI | BLTYPE_PRIMARY)
#define TID_CAT_STORAGECLASS  (4 << WIDTH_BLTYPE + WIDTH_TID_PI | BLTYPE_PRIMARY)
#define TID_CAT_TYPEQUALIFIER (5 << WIDTH_BLTYPE + WIDTH_TID_PI | BLTYPE_PRIMARY)

#define TID_P_ABSTRACT     (1 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_ASSERT       (2 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_BOOLEAN      (3 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_BREAK        (4 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_BYTE         (5 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_CASE         (6 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_CATCH        (7 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_CHAR         (8 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_CLASS        (9 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_CONST        (10 << WIDTH_BLTYPE | TID_CAT_TYPEQUALIFIER)
#define TID_P_CONTINUE     (11 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_DEFAULT      (12 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_DO           (13 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_DOUBLE       (14 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_ELSE         (15 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_ENUM         (16 << WIDTH_BLTYPE | TID_CAT_SUE)
#define TID_P_EXTENDS      (17 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_FALSE        (18 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_FINAL        (19 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_FINALLY      (20 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_FLOAT        (21 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_FOR          (22 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_GOTO         (23 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_IF           (24 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_IMPLEMENTS   (25 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_IMPORT       (26 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_INSTANCEOF   (27 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_INT          (28 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_INTERFACE    (29 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_LONG         (30 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_NATIVE       (31 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_NEW          (32 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_NULL         (33 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_PACKAGE      (34 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_PRIVATE      (35 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_PROTECTED    (36 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_PUBLIC       (37 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_RETURN       (38 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_SHORT        (39 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_STATIC       (40 << WIDTH_BLTYPE | TID_CAT_STORAGECLASS)
#define TID_P_STRICTFP     (41 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_SUPER        (42 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_SWITCH       (43 << WIDTH_BLTYPE | TID_CAT_STATEMENT)
#define TID_P_SYNCHRONIZED (44 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_THIS         (45 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_THROW        (46 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_THROWS       (47 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_TRANSIENT    (48 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_TRUE         (49 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_TRY          (50 << WIDTH_BLTYPE | BLTYPE_PRIMARY)
#define TID_P_VOID         (51 << WIDTH_BLTYPE | TID_CAT_TYPE)
#define TID_P_VOLATILE     (52 << WIDTH_BLTYPE | TID_CAT_TYPEQUALIFIER)
#define TID_P_WHILE        (53 << WIDTH_BLTYPE | TID_CAT_STATEMENT)

#endif
