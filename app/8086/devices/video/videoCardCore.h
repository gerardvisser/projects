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

#ifndef ___DEBUG_DEVICES_VIDEO__VIDEO_CARD_CORE_INCLUDED
#define ___DEBUG_DEVICES_VIDEO__VIDEO_CARD_CORE_INCLUDED

#include "dac.h"
#include "videoMode.h"
#include "../../memory.h"
#include "attributeConfig.h"
#include "graphicsRegisters.h"

class videoCardCore_t : public object_t {
private:
  graphicsRegisters_t* m_graphicsRegisters;
  attributeConfig_t* m_attributeConfig;
  videoConfig_t* m_videoConfig;
  cairo_t* m_drawingContext;
  videoMode_t* m_videoMode;
  uint32_t* m_videoBuffer;
  memory_t* m_memory;
  dac_t* m_dac;
  int m_attrIndex;
  int m_crtIndex;
  int m_dacReadIndex;
  int m_dacReadSubindex;
  int m_dacWriteIndex;
  int m_dacWriteSubindex;
  int m_gfxIndex;
  int m_seqIndex;
  uint8_t m_dacReadData[3];
  uint8_t m_dacWriteData[3];
  bool m_dacIndicesProtected;
  bool m_videoEnabled;

public:
  videoCardCore_t (memory_t* memory, cairo_surface_t* drawingSurface);
  virtual ~videoCardCore_t (void);

  virtual void clearScreen ();
  virtual void drawScreen (void);
  virtual uint8_t getAttributeData (void);
  virtual int getAttributeIndex (void);
  virtual uint8_t getCrtData (void);
  virtual int getCrtIndex (void);
  virtual uint8_t getDacData (void);
  virtual int getDacReadIndex (void);
  virtual int getDacWriteIndex (void);
  virtual uint8_t getGraphicsData (void);
  virtual int getGraphicsIndex (void);
  virtual uint8_t getSequencerData (void);
  virtual int getSequencerIndex (void);
  virtual bool getVideoEnabled (void);
  virtual void setAttributeData (uint8_t val);
  virtual void setAttributeIndex (int val);
  virtual void setCrtData (uint8_t val);
  virtual void setCrtIndex (int val);
  virtual void setDacData (uint8_t val);
  virtual void setDacReadIndex (int val);
  virtual void setDacWriteIndex (int val);
  virtual void setGraphicsData (uint8_t val);
  virtual void setGraphicsIndex (int val);
  virtual void setSequencerData (uint8_t val);
  virtual void setSequencerIndex (int val);
  virtual void setVideoEnabled (bool val);

private:
  void changeVideoMode (void);
  void initConfig (void);

  REFL_DECL
};

#endif
