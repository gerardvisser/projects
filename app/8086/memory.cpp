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
#include <string.h>
#include "memory.h"
#include "devices/video/memory/defaultVideoMem.h"

REFL_IMPL (memory_t, object_t)

#ifdef DEBUG_MODE
# define DBG_INDEX_CHECK(index, count) if (index < 0 || index + count > MEM_SIZE) errors_printMessageAndExit ("Index out of bounds")
#else
# define DBG_INDEX_CHECK(index, count)
#endif

#define HIGH_AREA_START  (VIDEO_AREA_START + VIDEO_AREA_SIZE)
#define MEM_SIZE         0x110000
#define VIDEO_AREA_SIZE  0x20000
#define VIDEO_AREA_START 0xA0000

memory_t::memory_t (void) {
  ___CBTPUSH;

  m_videoMem = new defaultVideoMem_t ();
  m_lbuf = (uint8_t*) mem_allocate ("uint8_t[]", VIDEO_AREA_START, PTYPE_INTERNAL);
  m_hbuf = (uint8_t*) mem_allocate ("uint8_t[]", MEM_SIZE - HIGH_AREA_START, PTYPE_INTERNAL);

  ___CBTPOP;
}

memory_t::~memory_t (void) {
  ___CBTPUSH;

  mem_release (m_lbuf);
  mem_release (m_hbuf);
  m_videoMem->dump ();

  ___CBTPOP;
}

int memory_t::read (int index, bool wide) const {
  ___CBTPUSH;

  int result;
  if (wide) {
    result = readWord (index);
  } else {
    result = readByte (index);
  }

  ___CBTPOP;
  return result;
}

int memory_t::readByte (int index) const {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index, 1);

  int result;
  switch (index >> 17) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
    result = m_lbuf[index];
    break;

  case 5:
    result = m_videoMem->readByte (index - VIDEO_AREA_START);
    break;

  default:
    result = m_hbuf[index - HIGH_AREA_START];
  }

  ___CBTPOP;
  return result;
}

int memory_t::readByteAsWord (int index) const {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index, 1);

  int result;
  switch (index >> 17) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
    result = (int8_t) m_lbuf[index];
    break;

  case 5:
    result = (int8_t) m_videoMem->readByte (index - VIDEO_AREA_START);
    break;

  default:
    result = (int8_t) m_hbuf[index - HIGH_AREA_START];
  }
  result &= 0xFFFF;

  ___CBTPOP;
  return result;
}

void memory_t::readBytes (uint8_t* dst, int index, uint16_t count) const {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index, count);

  switch (index >> 17) {
  case 0:
  case 1:
  case 2:
  case 3:
    memcpy (dst, m_lbuf + index, count);
    break;

  case 4:
    if (index + count <= VIDEO_AREA_START) {
      memcpy (dst, m_lbuf + index, count);
    } else {
      const int max = VIDEO_AREA_START - index;
      memcpy (dst, m_lbuf + index, max);
      for (int i = max; i < count; ++i) {
        dst[i] = m_videoMem->readByte (i - max);
      }
    }
    break;

  case 5:
    if (index + count <= HIGH_AREA_START) {
      index -= VIDEO_AREA_START;
      for (int i = 0; i < count; ++i) {
        dst[i] = m_videoMem->readByte (index + i);
      }
    } else {
      const int max = HIGH_AREA_START - index;
      index -= VIDEO_AREA_START;
      for (int i = 0; i < max; ++i) {
        dst[i] = m_videoMem->readByte (index + i);
      }
      memcpy (dst + max, m_hbuf, count - max);
    }
    break;

  default:
    memcpy (dst, m_hbuf + index - HIGH_AREA_START, count);
  }

  ___CBTPOP;
}

int memory_t::readWord (int index) const {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index, 2);

  int result;
  switch (index >> 17) {
  case 0:
  case 1:
  case 2:
  case 3:
    result = m_lbuf[index + 1];
    result <<= 8;
    result |= m_lbuf[index];
    break;

  case 4:
    if (index < VIDEO_AREA_START - 1) {
      result = m_lbuf[index + 1];
    } else {
      result = m_videoMem->readByte (0);
    }
    result <<= 8;
    result |= m_lbuf[index];
    break;

  case 5:
    if (index < HIGH_AREA_START - 1) {
      result = m_videoMem->readWord (index - VIDEO_AREA_START);
    } else {
      result = m_hbuf[0];
      result <<= 8;
      result |= m_videoMem->readByte (VIDEO_AREA_SIZE - 1);
    }
    break;

  default:
    index -= HIGH_AREA_START;
    result = m_hbuf[index + 1];
    result <<= 8;
    result |= m_hbuf[index];
  }

  ___CBTPOP;
  return result;
}

void memory_t::replaceVideoMem (videoMem_t* videoMem) {
  ___CBTPUSH;

  m_videoMem->dump ();
  m_videoMem = videoMem;
  videoMem->reserve ();

  ___CBTPOP;
}

void memory_t::write (int index, int val, bool wide) {
  ___CBTPUSH;

  if (wide) {
    writeWord (index, val);
  } else {
    writeByte (index, val);
  }

  ___CBTPOP;
}

void memory_t::writeByte (int index, int val) {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index, 1);

  switch (index >> 17) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
    m_lbuf[index] = val;
    break;

  case 5:
    m_videoMem->writeByte (index - VIDEO_AREA_START, val);
    break;

  default:
    m_hbuf[index - HIGH_AREA_START] = val;
  }

  ___CBTPOP;
}

void memory_t::writeBytes (int index, const uint8_t* src, uint16_t count) {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index, count);

  switch (index >> 17) {
  case 0:
  case 1:
  case 2:
  case 3:
    memcpy (m_lbuf + index, src, count);
    break;

  case 4:
    if (index + count <= VIDEO_AREA_START) {
      memcpy (m_lbuf + index, src, count);
    } else {
      const int max = VIDEO_AREA_START - index;
      memcpy (m_lbuf + index, src, max);
      for (int i = max; i < count; ++i) {
        m_videoMem->writeByte (i - max, src[i]);
      }
    }
    break;

  case 5:
    if (index + count <= HIGH_AREA_START) {
      index -= VIDEO_AREA_START;
      for (int i = 0; i < count; ++i) {
        m_videoMem->writeByte (index + i, src[i]);
      }
    } else {
      const int max = HIGH_AREA_START - index;
      index -= VIDEO_AREA_START;
      for (int i = 0; i < max; ++i) {
        m_videoMem->writeByte (index + i, src[i]);
      }
      memcpy (m_hbuf, src + max, count - max);
    }
    break;

  default:
    memcpy (m_hbuf + index - HIGH_AREA_START, src, count);
  }

  ___CBTPOP;
}

void memory_t::writeBytes (int index, int val, uint16_t count) {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index, count);

  switch (index >> 17) {
  case 0:
  case 1:
  case 2:
  case 3:
    memset (m_lbuf + index, val, count);
    break;

  case 4:
    if (index + count <= VIDEO_AREA_START) {
      memset (m_lbuf + index, val, count);
    } else {
      const int max = VIDEO_AREA_START - index;
      memset (m_lbuf + index, val, max);
      for (int i = max; i < count; ++i) {
        m_videoMem->writeByte (i - max, val);
      }
    }
    break;

  case 5:
    if (index + count <= HIGH_AREA_START) {
      index -= VIDEO_AREA_START;
      for (int i = 0; i < count; ++i) {
        m_videoMem->writeByte (index + i, val);
      }
    } else {
      const int max = HIGH_AREA_START - index;
      index -= VIDEO_AREA_START;
      for (int i = 0; i < max; ++i) {
        m_videoMem->writeByte (index + i, val);
      }
      memset (m_hbuf, val, count - max);
    }
    break;

  default:
    memset (m_hbuf + index - HIGH_AREA_START, val, count);
  }

  ___CBTPOP;
}

void memory_t::writeWord (int index, int val) {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index, 2);

  switch (index >> 17) {
  case 0:
  case 1:
  case 2:
  case 3:
    m_lbuf[index] = val;
    m_lbuf[index + 1] = val >> 8;
    break;

  case 4:
    if (index < VIDEO_AREA_START - 1) {
      m_lbuf[index + 1] = val >> 8;
    } else {
      m_videoMem->writeByte (0, val >> 8);
    }
    m_lbuf[index] = val;
    break;

  case 5:
    if (index < HIGH_AREA_START - 1) {
      m_videoMem->writeWord (index - VIDEO_AREA_START, val);
    } else {
      m_videoMem->writeByte (VIDEO_AREA_SIZE - 1, val);
      m_hbuf[0] = val >> 8;
    }
    break;

  default:
    index -= HIGH_AREA_START;
    m_hbuf[index] = val;
    m_hbuf[index + 1] = val >> 8;
  }

  ___CBTPOP;
}

void memory_t::writeWords (int index, int val, uint16_t count) {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index, 2 * count);

  const int hval = val >> 8;
  const int finalIndex = index + 2 * count;

  switch (index >> 17) {
  case 0:
  case 1:
  case 2:
  case 3:
    while (index < finalIndex) {
      m_lbuf[index + 1] = hval;
      m_lbuf[index] = val;
      index += 2;
    }
    break;

  case 4:
    if (finalIndex <= VIDEO_AREA_START) {
      while (index < finalIndex) {
        m_lbuf[index + 1] = hval;
        m_lbuf[index] = val;
        index += 2;
      }
    } else {
      int max = VIDEO_AREA_START - index >> 1;
      for (int i = 0; i < max; ++i) {
        m_lbuf[index + 1] = hval;
        m_lbuf[index] = val;
        index += 2;
      }
      if (index < VIDEO_AREA_START) {
        m_videoMem->writeByte (0, hval);
        m_lbuf[index] = val;
        index += 2;
      }
      max = finalIndex - index >> 1;
      index -= VIDEO_AREA_START;
      for (int i = 0; i < max; ++i) {
        m_videoMem->writeWord (index, val);
        index += 2;
      }
    }
    break;

  case 5:
    index -= VIDEO_AREA_START;
    if (finalIndex <= HIGH_AREA_START) {
      for (int i = 0; i < count; ++i) {
        m_videoMem->writeWord (index, val);
        index += 2;
      }
    } else {
      int max = VIDEO_AREA_SIZE - index >> 1;
      for (int i = 0; i < max; ++i) {
        m_videoMem->writeWord (index, val);
        index += 2;
      }
      if (index < VIDEO_AREA_SIZE) {
        m_videoMem->writeByte (VIDEO_AREA_SIZE - 1, val);
        m_hbuf[0] = hval;
        index += 2;
      }
      index -= VIDEO_AREA_SIZE;
      max = finalIndex - HIGH_AREA_START - index >> 1;
      for (int i = 0; i < max; ++i) {
        m_hbuf[index + 1] = hval;
        m_hbuf[index] = val;
        index += 2;
      }
    }
    break;

  default:
    index -= HIGH_AREA_START;
    for (int i = 0; i < count; ++i) {
      m_hbuf[index + 1] = hval;
      m_hbuf[index] = val;
      index += 2;
    }
  }

  ___CBTPOP;
}
