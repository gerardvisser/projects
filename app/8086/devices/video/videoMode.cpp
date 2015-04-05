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

#include <lombok/base/errors.h>
#include "serializers/all.h"
#include "videoCardConstants.h"
#include "videoMode.h"

I_REFL_IMPL (videoMode_t, object_t)

videoMode_t::videoMode_t (cairo_t* drawingContext, const videoConfig_t* videoConfig, const uint32_t* videoBuffer) : wm_drawingContext (drawingContext) {
  ___CBTPUSH;

  m_screenColumns = videoConfig->horizontalDisplayEnd;
  m_screenHeight = videoConfig->verticalDisplayEnd;
  m_screenWidth = videoConfig->characterWidth * m_screenColumns;
  m_yFactor = m_screenHeight > 240 ? 1 : 2;
  m_xFactor = m_screenWidth > 360 ? 1 : 2;
  m_xMargin = X_MARGIN + PIXEL_WIDTH * (SCREEN_WIDTH_PX - m_xFactor * m_screenWidth) / 2;
  m_yMargin = Y_MARGIN + PIXEL_WIDTH * (SCREEN_HEIGHT_PX - m_yFactor * m_screenHeight) / 2;
  m_serializer = createSerializer (videoConfig, videoBuffer);

  ___CBTPOP;
}

videoMode_t::~videoMode_t (void) {
  ___CBTPUSH;

  cairo_surface_destroy (m_sourceSurface);
  m_serializer->dump ();

  ___CBTPOP;
}

void videoMode_t::clearScreen (void) {
  ___CBTPUSH;

  cairo_save (wm_drawingContext);
  cairo_set_source_rgb (wm_drawingContext, 0, 0, 0);
  cairo_set_operator (wm_drawingContext, CAIRO_OPERATOR_SOURCE);
  cairo_paint (wm_drawingContext);
  cairo_restore (wm_drawingContext);

  ___CBTPOP;
}

serializer_t* videoMode_t::createSerializer (const videoConfig_t* videoConfig, const uint32_t* videoBuffer) {
  ___CBTPUSH;

  int startIndex;
  serializer_t* serializer;

  switch (videoConfig->memoryMapMode) {
  case MEMORY_MAP_MODE_A0000_BFFFF:
  case MEMORY_MAP_MODE_A0000_AFFFF:
    startIndex = 0;
    break;

  case MEMORY_MAP_MODE_B0000_B7FFF:
    startIndex = 0x10000 >> videoConfig->addressingMode;
    break;

  case MEMORY_MAP_MODE_B8000_BFFFF:
    startIndex = 0x18000 >> videoConfig->addressingMode;
  }

  switch (videoConfig->serializationMode) {
  case SERIALIZATION_MODE_SAME_BIT:
    serializer = new sameBitSerializer_t (videoBuffer + startIndex);
    break;

  case SERIALIZATION_MODE_ODD_EVEN:
    serializer = new oddEvenSerializer_t (videoBuffer + startIndex);
    break;

  case SERIALIZATION_MODE_SAME_BYTE:
    serializer = new sameByteSerializer_t (videoBuffer + startIndex);
  }

  ___CBTPOP;
  return serializer;
}

cairo_t* videoMode_t::createTempContext (cairo_surface_t* surface, double lineWidth) {
  ___CBTPUSH;

  cairo_t* context = cairo_create (surface);
  cairo_save (context);
  cairo_set_operator (context, CAIRO_OPERATOR_CLEAR);
  cairo_paint (context);
  cairo_restore (context);

  cairo_set_line_width (context, lineWidth);

  ___CBTPOP;
  return context;
}

int videoMode_t::regenBufferSize (void) {
  return m_regenBufferSize;
}

int videoMode_t::screenColumns (void) {
  return m_screenColumns;
}
