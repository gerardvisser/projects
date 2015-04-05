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

#ifndef ___LOMBOK__BASE__REFLECTION_INCLUDED
#define ___LOMBOK__BASE__REFLECTION_INCLUDED

#define classof(type) (&type::CLASS)

#define I_REFL_DECL \
  public: \
    virtual class_t* getClass (void) = 0; \
    static class_t CLASS;

#define REFL_DECL \
  public: \
    virtual class_t* getClass (void); \
    static class_t CLASS;

#define I_REFL_IMPL(type, superclass) ___I_REFL_IMPL (type, classof (superclass))

#define I_REFL_IMPL_NO_SUPERCLASS(type) ___I_REFL_IMPL (type, NULL)

#define REFL_IMPL(type, superclass) \
  I_REFL_IMPL (type, superclass) \
  ___GET_CLASS_IMPL (type)

#define REFL_IMPL_NO_SUPERCLASS(type) \
  I_REFL_IMPL_NO_SUPERCLASS (type) \
  ___GET_CLASS_IMPL (type)

#ifdef DEBUG_MODE
# define ___CLASS_NAME_DECL(type) static const char* const ___className = #type;
#else
# define ___CLASS_NAME_DECL(type)
#endif

#define ___GET_CLASS_IMPL(type) \
  class_t* type::getClass (void) { \
    return classof (type); \
  }

#define ___I_REFL_IMPL(type, superclass) \
  ___CLASS_NAME_DECL (type) \
  class_t type::CLASS (#type, sizeof (type), superclass);

class class_t;

#endif
