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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <lombok/base/errors.h>
#include <lombok/base/uchar.h>
#include "editor.h"

static int erase = -1;

static struct termios originalTermSettings;

typedef enum {
  TERMINAL_MODE_CANONICAL,
  TERMINAL_MODE_NONCANONICAL
} terminalMode_t;

static void setTerminalMode (terminalMode_t mode) {
  ___BTPUSH;

  if (erase == -1) {
    if (tcgetattr (STDIN_FILENO, &originalTermSettings)) {
      printf ("Could not read the terminal I/O attributes.\n");
      exit (EXIT_FAILURE);
    }
    erase = originalTermSettings.c_cc[VERASE];
  }

  struct termios settings = originalTermSettings;
  if (mode == TERMINAL_MODE_NONCANONICAL) {
    settings.c_lflag &= ~(ICANON | ECHO);
    settings.c_cc[VMIN] = 1;
  }
  if (tcsetattr (STDIN_FILENO, TCSANOW, &settings)) {
    printf ("Could not write the terminal I/O attributes.\n");
    exit (EXIT_FAILURE);
  }

  ___BTPOP;
}

void editor_edit (memory_t* memory, address_t* address) {
  ___BTPUSH;

  setTerminalMode (TERMINAL_MODE_NONCANONICAL);

  int c;
  printf ("%04X:%04X  ", address->segment (), address->offset ());

  do {
    int value = 0;
    int digitsRead = 0;
    int index = address->value ();
    printf ("%02X.", memory->readByte (index));
    do {
      c = getchar ();
      if (isxdigit (c) && digitsRead < 2) {
        value <<= 4;
        value |= uchar_hexDigitValue (c);
        putchar (c);
        ++digitsRead;
      } else if (c == erase && digitsRead > 0) {
        value >>= 4;
        printf ("\x1B[1D \x1B[1D");
        --digitsRead;
      }
    } while (!(c == '\n' || c == ' '));

    if (digitsRead > 0) {
      memory->writeByte (index, value);
    }
    if (c == ' ') {
      address->add (1);
      if ((address->offset () & 7) != 0) {
        char spaces[8];
        memset (spaces, ' ', 5 - digitsRead);
        spaces[5 - digitsRead] = 0;
        printf ("%s", spaces);
      } else {
        printf ("\n%04X:%04X  ", address->segment (), address->offset ());
      }
    }
  } while (c != '\n');

  setTerminalMode (TERMINAL_MODE_CANONICAL);
  putchar ('\n');

  ___BTPOP;
}
