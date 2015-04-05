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

#ifndef ___DEBUG_DEVICES_VIDEO__TEXT_MODE_INCLUDED
#define ___DEBUG_DEVICES_VIDEO__TEXT_MODE_INCLUDED

#include "attributeConfig.h"
#include "colour.h"
#include "dac.h"
#include "videoMode.h"

class textMode_t : public videoMode_t {
private:
  const colour_t* const m_palette;
  const int m_charHeight;
  const int m_charWidth;

public:
  textMode_t (cairo_t* drawingContext, const videoConfig_t* videoConfig, const uint32_t* videoBuffer, const attributeConfig_t* attributeConfig, const dac_t* dac);
  virtual ~textMode_t (void);

  virtual void drawScreen (void);

private:
  void drawCharacter (int code, int dx, int dy, int attribute);

  REFL_DECL
};

#endif
