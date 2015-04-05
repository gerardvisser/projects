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
#include <lombok/base/mem.h>
#include "font.h"
#include "textMode.h"
#include "videoCardConstants.h"

REFL_IMPL (textMode_t, videoMode_t)

static int getCharHeight (bool cols80);
static int getCharWidth (bool cols80);
static font_t* getFont (bool cols80);
static const colour_t* getPalette (const videoConfig_t* videoConfig, const attributeConfig_t* attributeConfig, const dac_t* dac);
static int getPixelWidth (bool cols80);

textMode_t::textMode_t (cairo_t* drawingContext,
                        const videoConfig_t* videoConfig,
                        const uint32_t* videoBuffer,
                        const attributeConfig_t* attributeConfig,
                        const dac_t* dac) :
    videoMode_t (drawingContext, videoConfig, videoBuffer),
    m_palette (getPalette (videoConfig, attributeConfig, dac)),
    m_charHeight (getCharHeight (videoConfig->horizontalDisplayEnd == 80)),
    m_charWidth (getCharWidth (videoConfig->horizontalDisplayEnd == 80)) {
  ___CBTPUSH;

  m_regenBufferSize = videoConfig->horizontalDisplayEnd == 40 ? 0x800 : 0x1000;

  const font_t* font = getFont (videoConfig->horizontalDisplayEnd == 80);
  const int pixWidth = getPixelWidth (videoConfig->horizontalDisplayEnd == 80);
  const int lineLength = CHAR_COUNT * font->width ();
  const int surfaceWidth = CHAR_COUNT * m_charWidth;
  const int surfaceHeight = PALETTE_SIZE * m_charHeight + 1;
  m_sourceSurface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, surfaceWidth, surfaceHeight);
  cairo_t* cr = createTempContext (m_sourceSurface, pixWidth);

  for (int paletteIndex = 0; paletteIndex < PALETTE_SIZE; ++paletteIndex) {
    cairo_set_source_rgb (cr, m_palette[paletteIndex].red, m_palette[paletteIndex].green, m_palette[paletteIndex].blue);

    for (int hline = 0; hline < font->height (); ++hline) {
      int col = 0;
      while (col < lineLength) {
        while (col < lineLength && font->getBit (col / font->width (), col % font->width (), hline) == 0) {
          ++col;
        }
        if (col < lineLength) {
          cairo_move_to (cr, pixWidth * col, pixWidth * (font->height () * paletteIndex + hline + Y_OFF));
          while (col < lineLength && font->getBit (col / font->width (), col % font->width (), hline) != 0) {
            ++col;
          }
          cairo_line_to (cr, pixWidth * col, pixWidth * (font->height () * paletteIndex + hline + Y_OFF));
          cairo_stroke (cr);
        }
      }
    }

  }
  cairo_destroy (cr);

  ___CBTPOP;
}

textMode_t::~textMode_t (void) {
  ___CBTPUSH;

  mem_release ((void*) m_palette);

  ___CBTPOP;
}

void textMode_t::drawCharacter (int code, int dx, int dy, int attribute) {
  ___CBTPUSH;

  int bgColour = attribute >> 4;
  int fgColour = attribute & 0xF;
  int sx = m_charWidth * code;
  int sy = m_charHeight * fgColour;

  cairo_set_source_rgb (wm_drawingContext, m_palette[bgColour].red, m_palette[bgColour].green, m_palette[bgColour].blue);
  cairo_rectangle (wm_drawingContext, dx, dy, m_charWidth, m_charHeight);
  cairo_fill (wm_drawingContext);

  cairo_set_source_surface (wm_drawingContext, m_sourceSurface, dx - sx, dy - sy);
  cairo_rectangle (wm_drawingContext, dx, dy, m_charWidth, m_charHeight);
  cairo_fill (wm_drawingContext);

  ___CBTPOP;
}

void textMode_t::drawScreen (void) {
  ___CBTPUSH;

  int index = 0;
  for (int y  = 0; y < 25; ++y) {
    for (int x  = 0; x < m_screenColumns; ++x) {
      int codeAttr = m_serializer->read (m_screenColumns * y + x);
      int attribute = codeAttr & 0xFF;
      int code = codeAttr >> 8;
      drawCharacter (code, m_charWidth * x + m_xMargin, m_charHeight * y + m_yMargin, attribute);
    }
  }

  ___CBTPOP;
}

static int getCharHeight (bool cols80) {
  ___BTPUSH;

  int result = getPixelWidth (cols80) * getFont (cols80)->height ();

  ___BTPOP;
  return result;
}

static int getCharWidth (bool cols80) {
  ___BTPUSH;

  int result = getPixelWidth (cols80) * getFont (cols80)->width ();

  ___BTPOP;
  return result;
}

static font_t* getFont (bool cols80) {
  ___BTPUSH;

  font_t* result = cols80 ? font_t::get9x16Font () : font_t::get8x8Font ();

  ___BTPOP;
  return result;
}

static const colour_t* getPalette (const videoConfig_t* videoConfig, const attributeConfig_t* attributeConfig, const dac_t* dac) {
  ___BTPUSH;

  colour_t* result = (colour_t*) mem_allocate ("colour_t[]", 16 * sizeof (colour_t), PTYPE_INTERNAL);
  for (int i = 0; i < 16; ++i) {
    dac->getColour (result + i, attributeConfig->getDacIndex (i));
  }

  ___BTPOP;
  return result;
}

static int getPixelWidth (bool cols80) {
  return cols80 ? PIXEL_WIDTH : 2 * PIXEL_WIDTH;
}
