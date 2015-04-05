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

#ifndef ___DEBUG_IOSYSTEM_INCLUDED
#define ___DEBUG_IOSYSTEM_INCLUDED

#include <lombok/util/arrayList.h>
#include "devices/device.h"

class iosystem_t : public object_t {
private:
  device_t** portDeviceMap;
  arrayList_t* devices;

public:
  iosystem_t (void);
  virtual ~iosystem_t (void);

  virtual bool addDevice (device_t* device);
  virtual int readByte (unsigned short port);
  virtual int readWord (unsigned short port);
  virtual void writeByte (unsigned short port, int val);
  virtual void writeWord (unsigned short port, int val);

  REFL_DECL
};

#endif
