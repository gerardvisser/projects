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

#include <lombok/base/errors.h>
#include <lombok/base/mem.h>
#include "realTimeClock.h"

REFL_IMPL (realTimeClock_t, device_t)

const int rtcports[] = {0x70, 0x71, -1};

#define SECONDS      0
#define MINUTES      2
#define HOURS        4
#define WEEKDAY      6
#define DAY_OF_MONTH 7
#define MONTH        8
#define YEAR         9
#define STATUS       10
#define DEFAULT      13

#define F_24_HOUR_MODE  0x02
#define F_BINARY_FORMAT 0x04

#define FLAGS_MASK (F_BINARY_FORMAT | F_24_HOUR_MODE)

#define getBcdValue(val) (16 * ((val) / 10) + (val) % 10)
#define getBinValue(val) (10 * ((val) >> 4) + ((val) & 0x0F))
#define getCurrentTime() (time (NULL) + m_delta)

realTimeClock_t::realTimeClock_t (void) : m_selected (DEFAULT), m_dateTime (0), m_delta (0) {
  ___CBTPUSH;

  m_date = (struct tm*) mem_allocate ("struct tm", sizeof (struct tm), PTYPE_INTERNAL);
  m_status[0] = 0x26; /* Just some number, making sure the highest bit is clear. */
  m_status[1] = F_24_HOUR_MODE; /* Calendar is in BCD format, 24-hour mode. */
  m_status[2] = 0;
  m_status[3] = 0x80; /* RTC has power. */
  m_status[4] = 0;
  m_status[5] = 0;

  ___CBTPOP;
}

realTimeClock_t::~realTimeClock_t (void) {
  ___CBTPUSH;

  mem_release (m_date);

  ___CBTPOP;
}

const int* realTimeClock_t::ports (void) {
  return rtcports;
}

int realTimeClock_t::readByte (unsigned short port) {
  ___CBTPUSH;

  int result = -1;
  if (port == 0x71) {
    if (m_selected < STATUS) {
      readDate ();
      switch (m_selected) {
      case SECONDS: result = m_date->tm_sec; break;
      case MINUTES: result = m_date->tm_min; break;
      case HOURS:
        result = m_date->tm_hour;
        if ((m_status[1] & F_24_HOUR_MODE) == 0) {
          if (result > 0) {
            if (result > 12) {
              result = result - 12 | 0x80;
            } else if (result == 12) {
              result |= 0x80;
            }
          } else {
            result = 12;
          }
        }
        break;
      case WEEKDAY: result = m_date->tm_wday; break;
      case DAY_OF_MONTH: result = m_date->tm_mday; break;
      case MONTH: result = m_date->tm_mon + 1; break;
      case YEAR: result = m_date->tm_year % 100; break;
      }
      if ((m_status[1] & F_BINARY_FORMAT) == 0) {
        if (result > 0x7F) {
          result &= 0x7F;
          result = getBcdValue (result);
          result |= 0x80;
        } else {
          result = getBcdValue (result);
        }
      }
    } else if (m_selected < STATUS + 6) {
      result = m_status[m_selected - STATUS];
    }
  }
  m_selected = DEFAULT;

  ___CBTPOP;
  return result;
}

void realTimeClock_t::readDate (void) {
  ___CBTPUSH;

  time_t currentTime = getCurrentTime ();
  if (currentTime - m_dateTime > 1) {
    m_dateTime = currentTime;
    localtime_r (&m_dateTime, m_date);
  }

  ___CBTPOP;
}

int realTimeClock_t::readWord (unsigned short port) {
  return -1;
}

void realTimeClock_t::writeByte (unsigned short port, int val) {
  ___CBTPUSH;

  switch (port) {
  case 0x70:
    m_selected = val;
    break;

  case 0x71:
    if (m_selected < STATUS) {

      struct tm date;
      time_t currentTime = getCurrentTime ();
      localtime_r (&currentTime, &date);
      bool bcdFormat = (m_status[1] & F_BINARY_FORMAT) == 0;
      bool amPmMode = (m_status[1] & F_24_HOUR_MODE) == 0;

      switch (m_selected) {
      case SECONDS:
        date.tm_sec = bcdFormat ? getBinValue (val) : val;
        break;
      case MINUTES:
        date.tm_min = bcdFormat ? getBinValue (val) : val;
        break;
      case HOURS:
        switch (amPmMode << 2 | bcdFormat << 1 | val > 0x7F) {
        case 2:
        case 3: val = getBinValue (val); break;
        case 4: if (val == 12) val = 0; break;
        case 5: val = val != 0x8C ? val + 12 & 0x7F : 12; break;
        case 6: val = val < 0x12 ? getBinValue (val) : 0; break;
        case 7: val = val != 0x92 ? getBinValue (val & 0x7F) + 12 : 12;
        }
        date.tm_hour = val;
        break;
      case DAY_OF_MONTH:
        date.tm_mday = bcdFormat ? getBinValue (val) : val;
        break;
      case MONTH:
        if (bcdFormat) val = getBinValue (val);
        date.tm_mon = val - 1;
        break;
      case YEAR:
        if (bcdFormat) val = getBinValue (val);
        date.tm_year = 100 * (date.tm_year / 100) + val;
      }
      time_t adjustedTime = mktime (&date);
      m_delta += adjustedTime - currentTime;
      m_dateTime = adjustedTime - 10;

    } else if (m_selected == STATUS + 1) {

      m_status[1] &= ~FLAGS_MASK;
      m_status[1] |= val & FLAGS_MASK;

    }
    m_selected = DEFAULT;
    break;
  }

  ___CBTPOP;
}

void realTimeClock_t::writeWord (unsigned short port, int val) {
  ___CBTPUSH;
  ___CBTPOP;
}
