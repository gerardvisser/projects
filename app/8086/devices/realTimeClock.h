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

#ifndef ___DEBUG_DEVICES__REAL_TIME_CLOCK_INCLUDED
#define ___DEBUG_DEVICES__REAL_TIME_CLOCK_INCLUDED

#include <time.h>
#include "device.h"

class realTimeClock_t : public device_t {
private:
  unsigned char m_status[8];
  struct tm* m_date;
  time_t m_dateTime;
  time_t m_delta;
  int m_selected;

public:
  realTimeClock_t (void);
  virtual ~realTimeClock_t (void);

  virtual const int* ports (void);
  virtual int readByte (unsigned short port);
  virtual int readWord (unsigned short port);
  virtual void writeByte (unsigned short port, int val);
  virtual void writeWord (unsigned short port, int val);

private:
  void readDate (void);

  REFL_DECL
};

#endif
