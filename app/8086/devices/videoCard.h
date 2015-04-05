/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2014, 2015 Gerard Visser.

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

#ifndef ___DEBUG_DEVICES__VIDEO_CARD_INCLUDED
#define ___DEBUG_DEVICES__VIDEO_CARD_INCLUDED

#include "device.h"
#include "video/monitor.h"
#include "video/videoCardCore.h"

class videoCard_t : public device_t {
private:
  cairo_surface_t* m_drawingSurface;
  videoCardCore_t* m_video;
  monitor_t* m_monitor;
  pthread_cond_t* m_condVar;
  pthread_mutex_t* m_mutex;
  pthread_t m_refreshThread;
  int m_refreshThreadState;
  bool m_attrWriteData;

public:
  videoCard_t (memory_t* memory);
  virtual ~videoCard_t (void);

  virtual const int* ports (void);
  virtual int readByte (unsigned short port);
  virtual int readWord (unsigned short port);
  virtual void writeByte (unsigned short port, int val);
  virtual void writeWord (unsigned short port, int val);

private:
  void suspendRefreshThread (void);
  void terminateRefreshThread (void);
  bool terminationRequested (void);

  static void* refreshThreadMain (void* videoCard);

  REFL_DECL
};

#endif
