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

#include <string.h>
#include <lombok/base/errors.h>
#include <lombok/base/mem.h>
#include "iosystem.h"

#define PORT_DEVICE_MAP_SIZE (0x10000 * sizeof (void*))

REFL_IMPL (iosystem_t, object_t)

iosystem_t::iosystem_t (void) {
  ___CBTPUSH;

  devices = new arrayList_t (8);
  portDeviceMap = (device_t**) mem_allocate ("device_t*[]", PORT_DEVICE_MAP_SIZE, PTYPE_INTERNAL);
  memset (portDeviceMap, 0, PORT_DEVICE_MAP_SIZE);

  ___CBTPOP;
}

iosystem_t::~iosystem_t (void) {
  ___CBTPUSH;

  mem_release (portDeviceMap);
  devices->dump ();

  ___CBTPOP;
}

/* Does not reserve device; does release it when this iosystem_t instance is released.  */
bool iosystem_t::addDevice (device_t* device) {
  ___CBTPUSH;

  int i = 0;
  bool deviceWasAdded;
  const int* ports = device->ports ();
  while (ports[i] != -1 && portDeviceMap[ports[i]] == 0) {
    portDeviceMap[ports[i]] = device;
    ++i;
  }
  deviceWasAdded = i > 0 && ports[i] == -1;
  if (deviceWasAdded) {
    devices->append (device);
  } else {
    --i;
    while (i > -1) {
      portDeviceMap[ports[i]] = 0;
      --i;
    }
  }

  ___CBTPOP;
  return deviceWasAdded;
}

int iosystem_t::readByte (unsigned short port) {
  ___CBTPUSH;

  int result = -1;
  device_t* device = portDeviceMap[port];
  if (device != 0) {
    result = device->readByte (port);
  }

  ___CBTPOP;
  return result;
}

int iosystem_t::readWord (unsigned short port) {
  ___CBTPUSH;

  int result = -1;
  device_t* device = portDeviceMap[port];
  if (device != 0) {
    result = device->readWord (port);
  }

  ___CBTPOP;
  return result;
}

void iosystem_t::writeByte (unsigned short port, int val) {
  ___CBTPUSH;

  device_t* device = portDeviceMap[port];
  if (device != 0) {
    device->writeByte (port, val & 0xFF);
  }

  ___CBTPOP;
}

void iosystem_t::writeWord (unsigned short port, int val) {
  ___CBTPUSH;

  device_t* device = portDeviceMap[port];
  if (device != 0) {
    device->writeWord (port, val & 0xFFFF);
  }

  ___CBTPOP;
}
