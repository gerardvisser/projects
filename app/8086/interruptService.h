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

#ifndef ___DEBUG_INTERRUPT_SERVICE_INCLUDED
#define ___DEBUG_INTERRUPT_SERVICE_INCLUDED

#include <lombok/base/class.h>

class interruptService_t : public object_t {
private:
  unsigned char* m_data;
  int m_serviceNumber;
  int m_dataSize;
  int m_csStart;
  int m_ipStart;
  bool m_errors;

public:
  interruptService_t (const char* filename);
  virtual ~interruptService_t (void);

  virtual int csStart (void);
  virtual const unsigned char* data (void);
  virtual int dataSize (void);
  virtual bool hasErrors (void);
  virtual int ipStart (void);
  virtual int serviceNumber (void);

  REFL_DECL
};

#endif
