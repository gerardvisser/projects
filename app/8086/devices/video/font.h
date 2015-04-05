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

#ifndef ___DEBUG_DEVICES_VIDEO__FONT_INCLUDED
#define ___DEBUG_DEVICES_VIDEO__FONT_INCLUDED

#include <lombok/base/class.h>

class font_t : public object_t {
private:
  const unsigned char* const m_buffer;
  const int m_bufferSize;
  const int m_height;
  const int m_width;

  font_t (const unsigned char* buffer, int width, int height);

public:
  virtual const unsigned char* buffer (void) const;
  virtual int bufferSize (void) const;
  virtual int charCount (void) const;
  virtual int getBit (int charCode, int x, int y) const;
  virtual int height (void) const;
  virtual int width (void) const;

  static font_t* get8x8Font (void);
  static font_t* get8x14Font (void);
  static font_t* get8x16Font (void);
  static font_t* get9x16Font (void);

  REFL_DECL
};

#endif
