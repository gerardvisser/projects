/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2015 Gerard Visser.

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
#include <lombok/base/int.h>
#include <lombok/base/mem.h>
#include "graphicsMode.h"
#include "videoCardConstants.h"

REFL_IMPL (graphicsMode_t, videoMode_t)

static int calcRegenBufferSize (const int width, const int height, const addressingMode_t addressingMode, const bool widePixels);
static colour_t* createPalette (const videoConfig_t* videoConfig, const attributeConfig_t* attributeConfig, const dac_t* dac);
static uint8_t* createPixelBuffer (const videoConfig_t* videoConfig);
static int getOddEvenTransfer (uint16_t value, int index);
static int getSameBitTransfer (uint16_t value, int index);
static int getSameByteTransfer (uint16_t value, int index);

graphicsMode_t::graphicsMode_t (cairo_t* drawingContext,
                                const videoConfig_t* videoConfig,
                                const uint32_t* videoBuffer,
                                const attributeConfig_t* attributeConfig,
                                const dac_t* dac) : videoMode_t (drawingContext, videoConfig, videoBuffer) {
  ___CBTPUSH;

  const int surfaceWidth = 2 * PIXEL_WIDTH * 0x400;
  const int surfaceHeight = 2 * PIXEL_WIDTH * (256 + Y_OFF) + 1;
  m_sourceSurface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, surfaceWidth, surfaceHeight);
  cairo_t* cr = createTempContext (m_sourceSurface, PIXEL_WIDTH);
  cairo_set_antialias (cr, CAIRO_ANTIALIAS_NONE);

  m_widePixels = videoConfig->widePixels;
  m_pixelsPerFetch = m_widePixels ? 2 : 4;
  m_regenBufferSize = calcRegenBufferSize (m_screenWidth, m_screenHeight, videoConfig->addressingMode, m_widePixels);

  if (videoConfig->cgaScanLineMode) {
    int s = m_screenWidth * m_screenHeight / (2 * m_pixelsPerFetch);
    m_oddScanLineOffset = 1 << int_bsr (s);
  } else {
    m_oddScanLineOffset = 0;
  }

  int x = 0;
  int y = 0;
  const uint8_t* buf = createPixelBuffer (videoConfig);
  const colour_t* palette = createPalette (videoConfig, attributeConfig, dac);
  const int xCount = videoConfig->widePixels ? 0x200 : 0x400;

  while (y < 0x100) {
    int oldX = x;
    int c = buf[xCount * y + x];
    cairo_set_source_rgba (cr, palette[c].red, palette[c].green, palette[c].blue, palette[c].alpha);
    cairo_move_to (cr, PIXEL_WIDTH * m_xFactor * x, PIXEL_WIDTH * m_yFactor * (y + Y_OFF));
    do {
      ++x;
    } while (x < xCount && buf[xCount * y + x] == c);
    cairo_line_to (cr, PIXEL_WIDTH * m_xFactor * x, PIXEL_WIDTH * m_yFactor * (y + Y_OFF));
    cairo_stroke (cr);
    if (m_yFactor == 2) {
      cairo_move_to (cr, PIXEL_WIDTH * m_xFactor * oldX, PIXEL_WIDTH * m_yFactor * (y + Y_OFF) + PIXEL_WIDTH);
      cairo_line_to (cr, PIXEL_WIDTH * m_xFactor * x, PIXEL_WIDTH * m_yFactor * (y + Y_OFF) + PIXEL_WIDTH);
      cairo_stroke (cr);
    }
    if (x == xCount) {
      x = 0;
      ++y;
    }
  }
  mem_release ((void*) palette);
  mem_release ((void*) buf);
  cairo_destroy (cr);

  ___CBTPOP;
}

graphicsMode_t::~graphicsMode_t (void) {
  ___CBTPUSH;


  ___CBTPOP;
}

void graphicsMode_t::drawScreen (void) {
  ___CBTPUSH;

  clearScreen ();
  const int xMax = m_widePixels ? m_screenWidth >> 1 : m_screenWidth >> 2;

  if (m_oddScanLineOffset == 0) {

    for (int y = 0; y < m_screenHeight; ++y) {
      for (int x = 0; x < xMax; ++x) {
        int index = m_serializer->read (xMax * y + x);
        double dy = m_yFactor * PIXEL_WIDTH * y + m_yMargin;
        double dx = m_pixelsPerFetch * m_xFactor * PIXEL_WIDTH * x + m_xMargin;
        double sy = m_yFactor * PIXEL_WIDTH * (index >> 8);
        double sx = m_pixelsPerFetch * m_xFactor * PIXEL_WIDTH * (index & 0xFF);
        cairo_set_source_surface (wm_drawingContext, m_sourceSurface, dx - sx, dy - sy);
        cairo_rectangle (wm_drawingContext, dx, dy, m_pixelsPerFetch * m_xFactor * PIXEL_WIDTH, m_yFactor * PIXEL_WIDTH);
        cairo_fill (wm_drawingContext);
      }
    }

  } else {

    const int yMax = m_screenHeight >> 1;
    for (int y = 0; y < yMax; ++y) {
      for (int x = 0; x < xMax; ++x) {
        int index = m_serializer->read (xMax * y + x);
        double dy = 2 * m_yFactor * PIXEL_WIDTH * y + m_yMargin;
        double dx = m_pixelsPerFetch * m_xFactor * PIXEL_WIDTH * x + m_xMargin;
        double sy = m_yFactor * PIXEL_WIDTH * (index >> 8);
        double sx = m_pixelsPerFetch * m_xFactor * PIXEL_WIDTH * (index & 0xFF);
        cairo_set_source_surface (wm_drawingContext, m_sourceSurface, dx - sx, dy - sy);
        cairo_rectangle (wm_drawingContext, dx, dy, m_pixelsPerFetch * m_xFactor * PIXEL_WIDTH, m_yFactor * PIXEL_WIDTH);
        cairo_fill (wm_drawingContext);

        dy += m_yFactor * PIXEL_WIDTH;
        index = m_serializer->read (xMax * y + x + m_oddScanLineOffset);
        sy = m_yFactor * PIXEL_WIDTH * (index >> 8);
        sx = m_pixelsPerFetch * m_xFactor * PIXEL_WIDTH * (index & 0xFF);
        cairo_set_source_surface (wm_drawingContext, m_sourceSurface, dx - sx, dy - sy);
        cairo_rectangle (wm_drawingContext, dx, dy, m_pixelsPerFetch * m_xFactor * PIXEL_WIDTH, m_yFactor * PIXEL_WIDTH);
        cairo_fill (wm_drawingContext);
      }
    }

  }

  ___CBTPOP;
}

static int calcRegenBufferSize (const int width, const int height, const addressingMode_t addressingMode, const bool widePixels) {
  const int pixelsPerByte = 8 >> addressingMode + widePixels;
  const int minBytesNeeded = width * height / pixelsPerByte;
  int result = 1 << int_bsr (minBytesNeeded) - 1;
  if (result < minBytesNeeded) {
    result <<= 1;
  }
  /* Problematisch als de waarde 0x10000 wordt, want maar in 2 bytes opgeslagen.
     Voor videomodus 0x12 zou deze waarde 0xA000 moeten wezen. Voor videomodus 0x13
     staat er 0x2000 (op oud systeem), maar dat is natuurlijk veel te klein! Het is
     de vraag of er ergens anders nog extra ruimte is... */
  return result;
}

static colour_t* createPalette (const videoConfig_t* videoConfig, const attributeConfig_t* attributeConfig, const dac_t* dac) {
  ___BTPUSH;

  colour_t* palette;
  int i;

  if (videoConfig->widePixels) {
    palette = (colour_t*) mem_allocate ("colour_t[]", 256 * sizeof (colour_t), PTYPE_ORDINARY);
    for (i = 0; i < 256; ++i) {
      dac->getColour (palette + i, i);
    }
  } else {
    palette = (colour_t*) mem_allocate ("colour_t[]", 16 * sizeof (colour_t), PTYPE_ORDINARY);
    for (i = 0; i < 16; ++i) {
      dac->getColour (palette + i, attributeConfig->getDacIndex (i));
    }
  }

  ___BTPOP;
  return palette;
}

static uint8_t* createPixelBuffer (const videoConfig_t* videoConfig) {
  ___BTPUSH;

  uint8_t* buffer = (uint8_t*) mem_allocate ("uint8_t[]", 0x40000, PTYPE_ORDINARY);
  int (* getTransfer)(uint16_t value, int index);
  switch (videoConfig->serializationMode) {
  case SERIALIZATION_MODE_SAME_BIT:
    getTransfer = getSameBitTransfer;
    break;

  case SERIALIZATION_MODE_ODD_EVEN:
    getTransfer = getOddEvenTransfer;
    break;

  case SERIALIZATION_MODE_SAME_BYTE:
    getTransfer = getSameByteTransfer;
  }
  if (videoConfig->widePixels) {
    for (int i = 0; i < 0x10000; ++i) {
      buffer[2*i] = getTransfer (i, 0) << 4 | getTransfer (i, 1);
      buffer[2*i + 1] = getTransfer (i, 2) << 4 | getTransfer (i, 3);
    }
  } else {
    for (int i = 0; i < 0x10000; ++i) {
      buffer[4*i] = getTransfer (i, 0);
      buffer[4*i + 1] = getTransfer (i, 1);
      buffer[4*i + 2] = getTransfer (i, 2);
      buffer[4*i + 3] = getTransfer (i, 3);
    }
  }

  ___BTPOP;
  return buffer;
}

static int getOddEvenTransfer (uint16_t value, int index) {
  value >>= 3 - index << 1;
  int result = value & 0x300;
  result >>= 6;
  result |= value & 0x3;
  return result;
}

static int getSameBitTransfer (uint16_t value, int index) {
  value >>= 3 - index;
  int result = (value & 0x1000) != 0;
  result <<= 1;
  result |= (value & 0x100) != 0;
  result <<= 1;
  result |= (value & 0x10) != 0;
  result <<= 1;
  result |= value & 0x1;
  return result;
}

static int getSameByteTransfer (uint16_t value, int index) {
  value >>= 3 - index << 2;
  return value & 0xF;
}
