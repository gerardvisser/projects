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

#ifndef ___LOMBOK__PARSING__BLOCK_INCLUDED
#define ___LOMBOK__PARSING__BLOCK_INCLUDED

#include <lombok/base/class.h>

namespace pars {

  class block_t : public object_t {
  private:
    const unsigned int _contextCode : 3;
    const unsigned int _errorCode : 2;
    const unsigned int _id : 27;

  public:
    block_t (int id, int contextCode, int errorCode);

    virtual int contextCode (void);
    virtual int errorCode (void);
    virtual int id (void);

    I_REFL_DECL
  };

}

#endif
