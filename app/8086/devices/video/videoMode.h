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

#ifndef ___DEBUG_DEVICES_VIDEO__VIDEO_MODE_INCLUDED
#define ___DEBUG_DEVICES_VIDEO__VIDEO_MODE_INCLUDED

#include <cairo.h>
#include "serializers/serializer.h"
#include "videoConfig.h"

class videoMode_t : public object_t {
protected:
  cairo_surface_t* m_sourceSurface;
  serializer_t* m_serializer;
  cairo_t* wm_drawingContext;
  int m_regenBufferSize;
  int m_screenColumns;
  int m_screenHeight;
  int m_screenWidth;
  int m_xFactor;
  int m_yFactor;
  int m_xMargin;
  int m_yMargin;

public:
  videoMode_t (cairo_t* drawingContext, const videoConfig_t* videoConfig, const uint32_t* videoBuffer);
  virtual ~videoMode_t (void);

  virtual void clearScreen (void);
  virtual void drawScreen (void) = 0;
  virtual int regenBufferSize (void);
  virtual int screenColumns (void);

protected:
  static serializer_t* createSerializer (const videoConfig_t* videoConfig, const uint32_t* videoBuffer);
  static cairo_t* createTempContext (cairo_surface_t* surface, double lineWidth);

  I_REFL_DECL
};

#endif
