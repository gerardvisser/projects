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

#include <string.h>
#include <lombok/base/errors.h>
#include <lombok/base/mem.h>
#include "graphicsMode.h"
#include "textMode.h"
#include "videoCardCore.h"
#include "memory/all.h"

REFL_IMPL (videoCardCore_t, object_t)

#ifdef DEBUG_MODE
static void logConfig (const videoConfig_t* videoConfig, const attributeConfig_t* attributeConfig, const graphicsRegisters_t* graphicsRegisters);
#endif

videoCardCore_t::videoCardCore_t (memory_t* memory, cairo_surface_t* drawingSurface) : m_memory (memory) {
  ___CBTPUSH;

  memory->reserve ();
  m_dac = new dac_t ();
  m_attributeConfig = new attributeConfig_t ();
  m_drawingContext = cairo_create (drawingSurface);
  cairo_set_antialias (m_drawingContext, CAIRO_ANTIALIAS_NONE);
  m_videoBuffer = (uint32_t*) mem_allocate ("uint32_t[]", 0x20000 * sizeof (uint32_t), PTYPE_INTERNAL);
  m_videoConfig = (videoConfig_t*) mem_allocate ("videoConfig_t", sizeof (videoConfig_t), PTYPE_INTERNAL);
  m_graphicsRegisters = (graphicsRegisters_t*) mem_allocate ("graphicsRegisters_t", sizeof (graphicsRegisters_t), PTYPE_INTERNAL);

  initConfig ();
  changeVideoMode ();
  clearScreen ();
  memory->writeByte (0x449, 3); /* Currently active video mode (Should be set by BIOS). */
  memory->writeWord (0x10C, 0x1600);
  memory->writeWord (0x10E, 0xC000); /* 0000:010C Pointer to start of font data table. */
  memory->writeByte (0x484, 24); /* Text rows - 1. */
  memory->writeWord (0x485, 0x10); /* Scan lines per character. */

  ___CBTPOP;
}

videoCardCore_t::~videoCardCore_t (void) {
  ___CBTPUSH;

  m_dac->dump ();
  m_memory->dump ();
  m_videoMode->dump ();
  m_attributeConfig->dump ();
  mem_release (m_videoBuffer);
  mem_release (m_videoConfig);
  cairo_destroy (m_drawingContext);
  mem_release (m_graphicsRegisters);

  ___CBTPOP;
}

void videoCardCore_t::changeVideoMode (void) {
  ___CBTPUSH;

  if (m_videoMode != NULL) {
    m_videoMode->dump ();
  }

  memset (m_videoBuffer, 0, 0x20000 * sizeof (uint32_t));

  videoMem_t* videoMem;
  switch (m_videoConfig->addressingMode) {
  case ADDRESSING_MODE_SEQUENTIAL: videoMem = new videoMem_t (m_videoBuffer, m_graphicsRegisters); break;
  case ADDRESSING_MODE_ODD_EVEN: videoMem = new chain2VideoMem_t (m_videoBuffer, m_graphicsRegisters); break;
  case ADDRESSING_MODE_CHAIN_4: videoMem = new chain4VideoMem_t (m_videoBuffer, m_graphicsRegisters); break;
  }
  m_memory->replaceVideoMem (videoMem);
  videoMem->dump ();

  if (m_videoConfig->graphicsMode) {
    m_videoMode = new graphicsMode_t (m_drawingContext, m_videoConfig, m_videoBuffer, m_attributeConfig, m_dac);
  } else {
    m_videoMode = new textMode_t (m_drawingContext, m_videoConfig, m_videoBuffer, m_attributeConfig, m_dac);
    m_memory->writeWords (0xB8000, 0x720, m_videoMode->regenBufferSize () / 2);
  }

  m_memory->writeWord (0x44A, m_videoMode->screenColumns ()); /* Screen width in text columns. */
  m_memory->writeWord (0x44C, m_videoMode->regenBufferSize ()); /* Length in bytes of video area. */
  m_memory->writeWord (0x44E, 0); /* Offset from video segment of active video memory page. */

  ___CBTPOP;
}

void videoCardCore_t::clearScreen () {
  ___CBTPUSH;

  m_videoMode->clearScreen ();

  ___CBTPOP;
}

void videoCardCore_t::drawScreen (void) {
  ___CBTPUSH;

  m_videoMode->drawScreen ();

  ___CBTPOP;
}

uint8_t videoCardCore_t::getAttributeData (void) {
  ___CBTPUSH;

  uint8_t result;
  if (m_attrIndex < 0x10) {
    result = m_attributeConfig->m_paletteRegs[m_attrIndex];
  } else {
    switch (m_attrIndex) {
    case 0x10:
      result = m_attributeConfig->m_bitsP5P4Select << 7 | m_videoConfig->widePixels << 6 | m_videoConfig->graphicsMode;
      break;

    case 0x14:
      result = m_attributeConfig->m_colourSelect;
      break;

    default:
      result = 0xFF;
    }
  }

  ___CBTPOP;
  return result;
}

int videoCardCore_t::getAttributeIndex (void) {
  return m_dacIndicesProtected << 5 | m_attrIndex;
}

uint8_t videoCardCore_t::getCrtData (void) {
  ___CBTPUSH;

  uint8_t result;
  switch (m_crtIndex) {
  case 0x01:
    result = m_videoConfig->horizontalDisplayEnd - 1;
    break;

  case 0x07: {
    int vde = m_videoConfig->verticalDisplayEnd - 1;
    result = ((vde & 0x200) != 0) << 6 | ((vde & 0x100) != 0) << 1;
  } break;

  case 0x12:
    result = m_videoConfig->verticalDisplayEnd - 1;
    break;

  case 0x17:
    result = !m_videoConfig->cgaScanLineMode;
    break;

  default:
    result = 0xFF;
  }

  ___CBTPOP;
  return result;
}

int videoCardCore_t::getCrtIndex (void) {
  return m_crtIndex;
}

uint8_t videoCardCore_t::getDacData (void) {
  ___CBTPUSH;

  if (m_dacReadSubindex == 0) {
    m_dac->getRGB (m_dacReadData, m_dacReadIndex);
    ++m_dacReadIndex;
    m_dacReadIndex &= 0xFF;
  }
  uint8_t result = m_dacReadData[m_dacReadSubindex];
  ++m_dacReadSubindex;
  m_dacReadSubindex %= 3;

  ___CBTPOP;
  return result;
}

int videoCardCore_t::getDacReadIndex (void) {
  return m_dacReadIndex;
}

int videoCardCore_t::getDacWriteIndex (void) {
  return m_dacWriteIndex;
}

uint8_t videoCardCore_t::getGraphicsData (void) {
  ___CBTPUSH;

  uint8_t result;
  switch (m_gfxIndex) {
  case 0:
    result = m_graphicsRegisters->setReset;
    break;

  case 1:
    result = m_graphicsRegisters->enableSetReset;
    break;

  case 2:
    result = m_graphicsRegisters->colourCompare;
    break;

  case 3:
    result = m_graphicsRegisters->logicalOperation << 3 | m_graphicsRegisters->rotateCount;
    break;

  case 4:
    result = m_graphicsRegisters->readPlaneSelect;
    break;

  case 5:
    result = m_videoConfig->serializationMode << 5 | (m_videoConfig->addressingMode == ADDRESSING_MODE_ODD_EVEN) << 4 |
             m_graphicsRegisters->readMode << 3 | m_graphicsRegisters->writeMode;
    break;

  case 6:
    result = m_videoConfig->memoryMapMode << 2 | (m_videoConfig->addressingMode == ADDRESSING_MODE_ODD_EVEN) << 1 | m_videoConfig->graphicsMode;
    break;

  case 7:
    result = m_graphicsRegisters->colourDontCare;
    break;

  case 8:
    result = m_graphicsRegisters->bitMask;
    break;

  default:
    result = 0xFF;
  }

  ___CBTPOP;
  return result;
}

int videoCardCore_t::getGraphicsIndex (void) {
  return m_gfxIndex;
}

uint8_t videoCardCore_t::getSequencerData (void) {
  ___CBTPUSH;

  uint8_t result;
  switch (m_seqIndex) {
  case 1:
    result = m_videoConfig->characterWidth == 8;
    break;

  case 2:
    result = m_graphicsRegisters->writePlaneEnable;
    break;

  case 4:
    switch (m_videoConfig->addressingMode) {
    case ADDRESSING_MODE_SEQUENTIAL: result = 6; break;
    case ADDRESSING_MODE_ODD_EVEN: result = 2; break;
    case ADDRESSING_MODE_CHAIN_4: result = 10;
    }
    break;

  default:
    result = 0xFF;
  }

  ___CBTPOP;
  return result;
}

int videoCardCore_t::getSequencerIndex (void) {
  return m_seqIndex;
}

bool videoCardCore_t::getVideoEnabled (void) {
  return m_videoEnabled;
}

void videoCardCore_t::initConfig (void) {
  ___CBTPUSH;

  m_videoMode = NULL;
  m_videoConfig->graphicsMode = false;
  m_videoConfig->serializationMode = SERIALIZATION_MODE_SAME_BYTE;
  m_videoConfig->addressingMode = ADDRESSING_MODE_CHAIN_4;
  m_videoConfig->memoryMapMode = MEMORY_MAP_MODE_B8000_BFFFF;
  m_videoConfig->horizontalDisplayEnd = 80;
  m_videoConfig->verticalDisplayEnd = 400;
  m_videoConfig->characterWidth = 9;
  m_videoConfig->cgaScanLineMode = false;
  m_videoConfig->widePixels = false;
  for (int i = 0; i < 16; ++i) {
    m_attributeConfig->m_paletteRegs[i] = i;
  }
  m_graphicsRegisters->setReset = 0;
  m_graphicsRegisters->enableSetReset = 0;
  m_graphicsRegisters->colourCompare = 0;
  m_graphicsRegisters->logicalOperation = 0;
  m_graphicsRegisters->rotateCount = 0;
  m_graphicsRegisters->readPlaneSelect = 0;
  m_graphicsRegisters->readMode = 0;
  m_graphicsRegisters->writeMode = 0;
  m_graphicsRegisters->colourDontCare = 0;
  m_graphicsRegisters->bitMask = 0xFF;
  m_graphicsRegisters->writePlaneEnable = 0xF;

  ___CBTPOP;
}

void videoCardCore_t::setAttributeData (uint8_t val) {
  ___CBTPUSH;

  if (m_attrIndex < 0x10) {
    if (!m_dacIndicesProtected) {
      m_attributeConfig->m_paletteRegs[m_attrIndex] = val & 0x3F;
    }
  } else {
    switch (m_attrIndex) {
    case 0x10:
      m_attributeConfig->m_bitsP5P4Select = (val & 0x80) != 0;
      m_videoConfig->widePixels = (val & 0x40) != 0;
      m_videoConfig->graphicsMode = val & 1;
      break;

    case 0x14:
      m_attributeConfig->m_colourSelect = val;
      break;
    }
  }

  ___CBTPOP;
}

void videoCardCore_t::setAttributeIndex (int val) {
  m_dacIndicesProtected = (val & 0x20) != 0;
  /* One source notes: clearing this bit (m_dacIndicesProtected) will cause
     the VGA-display data to become all 0. */
  m_attrIndex = val & 0x1F;
}

void videoCardCore_t::setCrtData (uint8_t val) {
  ___CBTPUSH;

  switch (m_crtIndex) {
  case 0x01:
    m_videoConfig->horizontalDisplayEnd = val + 1;
    break;

  case 0x07: {
    int vde = m_videoConfig->verticalDisplayEnd - 1 & 0xFF;
    vde |= ((val & 0x40) != 0) << 9 | ((val & 0x02) != 0) << 8;
    m_videoConfig->verticalDisplayEnd = vde + 1;
  } break;

  case 0x12: {
    int vde = m_videoConfig->verticalDisplayEnd - 1 & 0x300 | val;
    m_videoConfig->verticalDisplayEnd = vde + 1;
  } break;

  case 0x17:
    m_videoConfig->cgaScanLineMode = (val & 1) == 0;
  }

  ___CBTPOP;
}

void videoCardCore_t::setCrtIndex (int val) {
  m_crtIndex = val;
}

void videoCardCore_t::setDacData (uint8_t val) {
  ___CBTPUSH;

  m_dacWriteData[m_dacWriteSubindex] = val & 0x3F;
  ++m_dacWriteSubindex;
  if (m_dacWriteSubindex == 3) {
    m_dac->setRGB (m_dacWriteIndex, m_dacWriteData);
    m_dacWriteSubindex = 0;
    ++m_dacWriteIndex;
    m_dacWriteIndex &= 0xFF;
  }

  ___CBTPOP;
}

void videoCardCore_t::setDacReadIndex (int val) {
  m_dacReadSubindex = 0;
  m_dacReadIndex = val;
}

void videoCardCore_t::setDacWriteIndex (int val) {
  m_dacWriteSubindex = 0;
  m_dacWriteIndex = val;
}

void videoCardCore_t::setGraphicsData (uint8_t val) {
  ___CBTPUSH;

  switch (m_gfxIndex) {
  case 0:
    m_graphicsRegisters->setReset = val & 0xF;
    break;

  case 1:
    m_graphicsRegisters->enableSetReset = val & 0xF;
    break;

  case 2:
    m_graphicsRegisters->colourCompare = val & 0xF;
    break;

  case 3:
    m_graphicsRegisters->logicalOperation = val >> 3 & 3;
    m_graphicsRegisters->rotateCount = val & 7;
    break;

  case 4:
    m_graphicsRegisters->readPlaneSelect = val & 3;
    break;

  case 5:
    m_videoConfig->serializationMode = (serializationMode_t) (val >> 5 & 3);
    if (m_videoConfig->serializationMode == 3) {
      m_videoConfig->serializationMode = SERIALIZATION_MODE_SAME_BYTE;
    }
    if (val & 0x10) {
      m_videoConfig->addressingMode = ADDRESSING_MODE_ODD_EVEN;
    }
    m_graphicsRegisters->readMode = val >> 3 & 1;
    m_graphicsRegisters->writeMode = val & 3;
    break;

  case 6:
    m_videoConfig->memoryMapMode = (memoryMapMode_t) (val >> 2 & 3);
    if (val & 2) {
      m_videoConfig->addressingMode = ADDRESSING_MODE_ODD_EVEN;
    }
    m_videoConfig->graphicsMode = val & 1;
    break;

  case 7:
    m_graphicsRegisters->colourDontCare = val & 0xF;
    break;

  case 8:
    m_graphicsRegisters->bitMask = val;
  }

  ___CBTPOP;
}

void videoCardCore_t::setGraphicsIndex (int val) {
  m_gfxIndex = val;
}

void videoCardCore_t::setSequencerData (uint8_t val) {
  ___CBTPUSH;

  switch (m_seqIndex) {
  case 1:
    m_videoConfig->characterWidth = val & 1 ? 8 : 9;
    break;

  case 2:
    m_graphicsRegisters->writePlaneEnable = val & 0xF;
    break;

  case 4:
    switch (val & 0xC) {
    case 0: m_videoConfig->addressingMode = ADDRESSING_MODE_ODD_EVEN; break;
    case 4: m_videoConfig->addressingMode = ADDRESSING_MODE_SEQUENTIAL; break;
    default: m_videoConfig->addressingMode = ADDRESSING_MODE_CHAIN_4;
    }
    break;
  }

  ___CBTPOP;
}

void videoCardCore_t::setSequencerIndex (int val) {
  m_seqIndex = val;
}

void videoCardCore_t::setVideoEnabled (bool val) {
  ___CBTPUSH;

  if (!m_videoEnabled & val) {
    changeVideoMode ();
  }
  m_videoEnabled = val;

  ___CBTPOP;
}


#ifdef DEBUG_MODE

#include <stdio.h>

static void logConfig (const videoConfig_t* videoConfig, const attributeConfig_t* attributeConfig, const graphicsRegisters_t* graphicsRegisters) {
  ___BTPUSH;

  printf ("\x1B[38;5;220m\n");

  printf ("Serialization mode: ");
  switch (videoConfig->serializationMode) {
  case SERIALIZATION_MODE_SAME_BIT: printf ("SERIALIZATION_MODE_SAME_BIT"); break;
  case SERIALIZATION_MODE_ODD_EVEN: printf ("SERIALIZATION_MODE_ODD_EVEN"); break;
  case SERIALIZATION_MODE_SAME_BYTE: printf ("SERIALIZATION_MODE_SAME_BYTE"); break;
  default: printf ("Unknown");
  }
  printf ("\nAddressing mode: ");
  switch (videoConfig->addressingMode) {
  case ADDRESSING_MODE_SEQUENTIAL: printf ("ADDRESSING_MODE_SEQUENTIAL"); break;
  case ADDRESSING_MODE_ODD_EVEN: printf ("ADDRESSING_MODE_ODD_EVEN"); break;
  case ADDRESSING_MODE_CHAIN_4: printf ("ADDRESSING_MODE_CHAIN_4"); break;
  default: printf ("Unknown");
  }
  printf ("\nMemory map mode: ");
  switch (videoConfig->memoryMapMode) {
  case MEMORY_MAP_MODE_A0000_BFFFF: printf ("MEMORY_MAP_MODE_A0000_BFFFF"); break;
  case MEMORY_MAP_MODE_A0000_AFFFF: printf ("MEMORY_MAP_MODE_A0000_AFFFF"); break;
  case MEMORY_MAP_MODE_B0000_B7FFF: printf ("MEMORY_MAP_MODE_B0000_B7FFF"); break;
  case MEMORY_MAP_MODE_B8000_BFFFF: printf ("MEMORY_MAP_MODE_B8000_BFFFF"); break;
  default: printf ("Unknown");
  }
  printf ("\nGraphics: %d, CGA scan line mode: %d\n", videoConfig->graphicsMode, videoConfig->cgaScanLineMode);
  printf ("Resolution: %d × %d, ", videoConfig->horizontalDisplayEnd * videoConfig->characterWidth, videoConfig->verticalDisplayEnd);
  printf ("Colours: %d\n", videoConfig->widePixels ? 256 : 16);
  if (!videoConfig->widePixels) {
    printf ("DAC indices: %02X", attributeConfig->getDacIndex (0));
    for (int i = 1; i < 16; ++i) {
      printf (", %02X", attributeConfig->getDacIndex (i));
    }
    printf ("\n");
  }
  printf ("         Set/Reset: %02X             Read Mode: %02X\n", graphicsRegisters->setReset, graphicsRegisters->readMode);
  printf ("  Enable Set/Reset: %02X            Write Mode: %02X\n", graphicsRegisters->enableSetReset, graphicsRegisters->writeMode);
  printf ("    Colour Compare: %02X     Colour Don't Care: %02X\n", graphicsRegisters->colourCompare, graphicsRegisters->colourDontCare);
  printf (" Logical Operation: %02X              Bit Mask: %02X\n", graphicsRegisters->logicalOperation, graphicsRegisters->bitMask);
  printf ("      Rotate Count: %02X    Write Plane Enable: %02X\n", graphicsRegisters->rotateCount, graphicsRegisters->writePlaneEnable);
  printf (" Read Plane Select: %02X\n", graphicsRegisters->readPlaneSelect);

  printf ("\x1B[0m\n");

  ___BTPOP;
}

#endif
