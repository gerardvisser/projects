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

#ifndef ___LOMBOK__PARSING__TOKEN_I_DS__TOKEN_I_DS_INCLUDED
#define ___LOMBOK__PARSING__TOKEN_I_DS__TOKEN_I_DS_INCLUDED

#include <lombok/parsing/blockTypes.h>

/* Number tokens: */
#define TID_N_DEC   (0 << WIDTH_BLTYPE | BLTYPE_NUMBER)
#define TID_N_FLOAT (1 << WIDTH_BLTYPE | BLTYPE_NUMBER)
#define TID_N_HEX   (2 << WIDTH_BLTYPE | BLTYPE_NUMBER)
#define TID_N_OCT   (3 << WIDTH_BLTYPE | BLTYPE_NUMBER)


/* Secondary tokens: */
#define WIDTH_TID_SI 6 /* To make them unique. */

#define TID_S_AT                (1 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_BACKSLASH         (2 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_BRACE_CLOSE       (3 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_BRACE_OPEN        (4 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_BRACKET_CLOSE     (5 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_BRACKET_OPEN      (6 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_COLON             (7 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_COMMA             (8 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_DOUBLE_COLON      (9 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_DOUBLE_HASH       (10 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_ELLIPSIS          (11 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_HASH              (12 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_PARENTHESIS_CLOSE (13 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_PARENTHESIS_OPEN  (14 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_QUESTION_MARK     (15 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_SEMICOLON         (16 << WIDTH_BLTYPE | BLTYPE_SECONDARY)

#define TID_S_ADD               (17 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_AMPERSAND         (18 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_AND               (19 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_ARROW             (20 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_ASTERISK          (21 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_DEC               (22 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_DIV               (23 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_DOT               (24 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_EQUAL             (25 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_EXCLAMATION_MARK  (26 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_GREATER           (27 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_HAT               (28 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_INC               (29 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_LESS              (30 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_LOG_AND           (31 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_LOG_OR            (32 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_MINUS             (33 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_MOD               (34 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_MOV               (35 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_MUCH_GREATER      (36 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_MUCH_LESS         (37 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_MUL               (38 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_NOT_EQUAL         (39 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_NOT_GREATER       (40 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_NOT_LESS          (41 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_OR                (42 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_PERCENT           (43 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_PIPE              (44 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_PLUS              (45 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_SAL               (46 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_SAR               (47 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_SHR               (48 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_SHRWOA            (49 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_SLASH             (50 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_SUB               (51 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_TILDE             (52 << WIDTH_BLTYPE | BLTYPE_SECONDARY)
#define TID_S_XOR               (53 << WIDTH_BLTYPE | BLTYPE_SECONDARY)

#endif
