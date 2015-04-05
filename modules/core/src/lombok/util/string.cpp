/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2012, 2013 Gerard Visser.

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
#include <lombok/util/string.h>

REFL_IMPL (string_t, charSequence_t)

static const uchar_t iso8859_2[] = {0x00A0, 0x0104, 0x02D8, 0x0141, 0x00A4, 0x013D, 0x015A, 0x00A7,
                                    0x00A8, 0x0160, 0x015E, 0x0164, 0x0179, 0x00AD, 0x017D, 0x017B,
                                    0x00B0, 0x0105, 0x02DB, 0x0142, 0x00B4, 0x013E, 0x015B, 0x02C7,
                                    0x00B8, 0x0161, 0x015F, 0x0165, 0x017A, 0x02DD, 0x017E, 0x017C,
                                    0x0154, 0x00C1, 0x00C2, 0x0102, 0x00C4, 0x0139, 0x0106, 0x00C7,
                                    0x010C, 0x00C9, 0x0118, 0x00CB, 0x011A, 0x00CD, 0x00CE, 0x010E,
                                    0x0110, 0x0143, 0x0147, 0x00D3, 0x00D4, 0x0150, 0x00D6, 0x00D7,
                                    0x0158, 0x016E, 0x00DA, 0x0170, 0x00DC, 0x00DD, 0x0162, 0x00DF,
                                    0x0155, 0x00E1, 0x00E2, 0x0103, 0x00E4, 0x013A, 0x0107, 0x00E7,
                                    0x010D, 0x00E9, 0x0119, 0x00EB, 0x011B, 0x00ED, 0x00EE, 0x010F,
                                    0x0111, 0x0144, 0x0148, 0x00F3, 0x00F4, 0x0151, 0x00F6, 0x00F7,
                                    0x0159, 0x016F, 0x00FA, 0x0171, 0x00FC, 0x00FD, 0x0163, 0x02D9};

static int utf8strlen (const char* str) {
  ___BTPUSH;

  int i = 0;
  int len = 0;
  while (str[i] != 0) {
    if ((str[i] & 0xC0) !=  0x80) {
      ++len;
    }
    ++i;
  }

  ___BTPOP;
  return len;
}

#define BUF_SIZE sizeof (uchar_t) * len

string_t::string_t (const char* str, int characterEncoding) {
  ___CBTPUSH;

  int len = characterEncoding != CHAR_ENC_UTF8 ? strlen (str) : utf8strlen (str);
  if (len > 0) {
    buf = (uchar_t*) mem_allocate ("uchar_t[]", BUF_SIZE, PTYPE_INTERNAL);

    switch (characterEncoding) {
    case CHAR_ENC_UTF8: {
      int j = 0;
      for (int i = 0; i < len; ++i) {
        int k = (unsigned char) str[j] >> 5;
        switch (k) {
        case 6: {
          buf[i] = (str[j] & 0x1F) << 6;
          ++j;
          buf[i] |= str[j] & 0x3F;
        } break;

        case 7: {
          buf[i] = (str[j] & 0x0F) << 6;
          ++j;
          buf[i] |= (str[j] & 0x3F) << 6;
          ++j;
          buf[i] |= str[j] & 0x3F;
        } break;

        default: {
          buf[i] = str[j];
        }
        }
        ++j;
      }
    } break;

    case CHAR_ENC_ISO8859_2: {
      for (int i = 0; i < len; ++i) {
        if ((unsigned char) str[i] > 0x9F) {
          buf[i] = iso8859_2[(unsigned char) str[i] - 0xA0];
        } else {
          buf[i] = (unsigned char) str[i];
        }
      }
    } break;

    default: { /* CHAR_ENC_ISO8859_1 */
      for (int i = 0; i < len; ++i) {
        buf[i] = (unsigned char) str[i];
      }
    }
    }
  } else {
    buf = NULL;
  }
  _length = len;
  _hashCode = charSequence_t::hashCode ();

  ___CBTPOP;
}

string_t::string_t (const uchar_t* str, int len) {
  ___CBTPUSH;

  if (len != 0) {
    int size = BUF_SIZE;
    buf = (uchar_t*) mem_allocate ("uchar_t[]", size, PTYPE_INTERNAL);
    memcpy (buf, str, size);
  } else {
    buf = NULL;
  }
  _length = len;
  _hashCode = charSequence_t::hashCode ();

  ___CBTPOP;
}

unsigned int string_t::hashCode (void) {
  return _hashCode;
}

string_t* string_t::substring (int startIndex, int length) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (startIndex < 0 || startIndex > _length) {
    errors_printMessageAndExit ("Index out of bounds");
  } else if (length > _length - startIndex) {
    errors_printMessageAndExit ("Length too large");
  } else if (length < 0) {
    errors_printMessageAndExit ("Length cannot be negative");
  }
#endif

  string_t* result = new string_t (buf + startIndex, length);

  ___CBTPOP;
  return result;
}
