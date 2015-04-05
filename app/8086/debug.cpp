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
#include <string.h>
#include <lombok/base/all.h>
#include <lombok/io/fileInputStream.h>
#include <lombok/io/fileOutputStream.h>
#include "devices/realTimeClock.h"
#include "devices/videoCard.h"
#include "devices/video/font.h"
#include "assembler.h"
#include "editor.h"
#include "interruptService.h"
#include "iosystem.h"
#include "printer.h"
#include "processor.h"

memory_t* memory;
printer_t* printer;
registers_t* registers;
assembler_t* assembler;
processor_t* processor;
lineInputReader_t* input;
address_t* aAddress;
address_t* dAddress;
address_t* eAddress;
address_t* uAddress;

int filenameIndex;

int commandA (void);
int commandDorU (address_t* address, int* len);
int commandE (void);
int commandG (void);
int commandH (void);
int commandN (void);
int commandQuestionMark (void);
int commandR (void);
int commandT (void);
int commandW (void);
void finishOff (void);
void init (void);
void loadFonts (void);
void loadInterruptServiceRoutines (void);
int noFurtherInputAllowed (int errorCode);
void printErrorMessage (int errorCode);
void printRegisters (void);
void printStreamError (stream_t* stream);
void processCommandlineArgs (int argc, char** args);
int readAddress (address_t* address, int firstChar);
int readWord (int* dst);

int main (int argc, char** args, char** env) {
  ___BTPUSH;
  init ();

  processCommandlineArgs (argc, args);

  int len;
  bool done = false;
  while (!done) {
    putchar ('-');
    int errorCode = 0;
    input->getLineFromStdin ();
    switch (input->read ()) {
    case 0: break;

    case 'A': errorCode = commandA (); break;

    case 'D':
      len = 0x80;
      errorCode = commandDorU (dAddress, &len);
      if (errorCode == 0) {
        printer->printMemory (dAddress, len);
      }
      break;

    case 'E': errorCode = commandE (); break;

    case 'G': errorCode = commandG (); break;

    case 'H': errorCode = commandH (); break;

    case 'N': errorCode = commandN (); break;

    case 'Q': done = true; break;

    case 'R': errorCode = commandR (); break;

    case 'T': errorCode = commandT (); break;

    case 'U':
      len = 0x20;
      errorCode = commandDorU (uAddress, &len);
      if (errorCode == 0) {
        printer->printCode (uAddress, len);
      }
      break;

    case 'W': errorCode = commandW (); break;

    case '?': errorCode = commandQuestionMark (); break;

    default:
      input->rewind ();
      errorCode = 1;
    }
    if (errorCode != 0) {
      printErrorMessage (errorCode);
    }
  }

  finishOff ();
  ___BTPOP;
  return 0;
}

int commandA (void) {
  ___BTPUSH;

  int errorCode = 0;
  int c = input->readNextNonspace ();
  if (c != 0) {
    errorCode = readAddress (aAddress, c);
    errorCode = noFurtherInputAllowed (errorCode);
  }
  if (errorCode == 0) {
    assembler->assemble (aAddress);
  }

  ___BTPOP;
  return errorCode;
}

int commandDorU (address_t* address, int* len) {
  ___BTPUSH;

  int errorCode = 0;
  int c = input->readNextNonspace ();
  if (c != 0) {
    errorCode = readAddress (address, c);

    if (errorCode == 0) {
      c = input->readNextNonspace ();
      if (c != 0) {
        if (c == 'L') {
          c = input->read ();
          if (c != ' ') input->rewind ();
          errorCode = readWord (len);
        } else {
          input->rewind ();
          errorCode = readWord (len);
          len[0] = len[0] + 1 - address->offset ();
        }
        if (errorCode == 0) {
          errorCode = len[0] <= 0 || address->offset () + len[0] > 0x10000;
        }
        errorCode = noFurtherInputAllowed (errorCode);
      }
    }
  }

  if (address->offset () + len[0] > 0x10000) {
    len[0] = 0x10000 - address->offset ();
  }

  ___BTPOP;
  return errorCode;
}

int commandE (void) {
  ___BTPUSH;

  int errorCode = 0;
  int c = input->readNextNonspace ();
  if (c != 0) {
    errorCode = readAddress (eAddress, c);
    errorCode = noFurtherInputAllowed (errorCode);
  }
  if (errorCode == 0) {
    editor_edit (memory, eAddress);
  }

  ___BTPOP;
  return errorCode;
}

int commandG (void) {
  ___BTPUSH;

  int offset = -1;
  int segment = -1;
  int errorCode = 0;
  int c = input->readNextNonspace ();
  if (c != 0) {
    address_t address (registers->cs ());
    errorCode = readAddress (&address, c);
    errorCode = noFurtherInputAllowed (errorCode);
    segment = address.segment ();
    offset = address.offset ();
  }
  if (errorCode == 0) {
    if (offset == -1) {
      printf ("\x1B[31mNot yet implemented.\x1B[0m\n");
    } else {
      while (!(offset == registers->ip () && segment == registers->cs ())) {
        processor->executeNextInstruction ();
      }
      putchar ('\n');
      printRegisters ();
    }
  }

  ___BTPOP;
  return errorCode;
}

int commandH (void) {
  ___BTPUSH;

  int x, y;
  int errorCode = 0;
  int c = input->read ();
  errorCode = c == 0;
  if (errorCode == 0) {
    if (c != ' ') input->rewind ();
    errorCode = readWord (&x);
    if (errorCode == 0) {
      errorCode = input->read () != ' ';
      if (errorCode == 0) {
        errorCode = readWord (&y);
        errorCode = noFurtherInputAllowed (errorCode);
        if (errorCode == 0) {
          printf ("%04X  %04X\n", (short) (x + y), (unsigned short) (x - y));
        }
      }
    }
  }

  ___BTPOP;
  return errorCode;
}

int commandN (void) {
  ___BTPUSH;

  int len = 0;
  int index = filenameIndex + 1;
  int c = input->readNextNonspace (false);
  while (c != 0 && len < 127) {
    memory->writeByte (index + len, c);
    c = input->read (false);
    ++len;
  }
  memory->writeByte (filenameIndex, len);

  ___BTPOP;
  return 0;
}

int commandQuestionMark (void) {
  ___BTPUSH;

  int errorCode = noFurtherInputAllowed (0);
  if (errorCode == 0) {
    printf ("assemble     A [address]\n");
    printf ("dump         D [range]\n");
    printf ("edit         E [address]\n");
    printf ("go           G [address]\n");
    printf ("hex          H value1 value2\n");
    printf ("name         N filename\n");
    printf ("quit         Q\n");
    printf ("register     R [register]\n");
    printf ("trace        T\n");
    printf ("unassemble   U [range]\n");
    printf ("write        W\n");
  }

  ___BTPOP;
  return errorCode;
}

int commandR (void) {
  ___BTPUSH;

  char buf[2];
  int errorCode = 0;

  buf[0] = input->readNextNonspace ();
  if (buf[0] != 0) {
    buf[1] = input->read ();

    int i = 0;
    const char* const regs = "AX\0CX\0DX\0BX\0SP\0BP\0SI\0DI\0ES\0CS\0SS\0DS\0IP\0";
    while (!(regs[i] == 0 || strncmp (buf, regs + i, 2) == 0)) {
      i += 3;
    }
    errorCode = regs[i] == 0;

    if (errorCode == 0) {
      errorCode = noFurtherInputAllowed (errorCode);
      if (errorCode == 0) {
        printf ("%s", regs + i);
        i /= 3;
        int value = i > 7 ? i == 12 ? registers->ip () : registers->seg (i-8) : registers->gen (i);
        printf (" %04X\n:", value);

        input->getLineFromStdin ();
        buf[0] = input->read ();
        if (buf[0] != 0) {
          input->rewind ();
          errorCode = readWord (&value);
          errorCode = noFurtherInputAllowed (errorCode);
          if (errorCode == 0) {
            if (i == 12) {
              processor->setInstructionPointer (value);
            } else if (i > 7) {
              processor->setSegmentRegister (i-8, value);
            } else {
              processor->setGeneralPurposeRegister (i, value);
            }
          }
        }
      }
    } else {
      input->rewind (2);
    }
  } else {
    printRegisters ();
  }

  ___BTPOP;
  return errorCode;
}

int commandT (void) {
  ___BTPUSH;

  int errorCode = noFurtherInputAllowed (0);
  if (errorCode == 0) {
    processor->setTrap ();
    processor->executeNextInstruction ();
    processor->clearTrap ();
    putchar ('\n');
    printRegisters ();
  }

  ___BTPOP;
  return errorCode;
}

int commandW (void) {
  ___BTPUSH;

  int errorCode = noFurtherInputAllowed (0);
  if (errorCode == 0) {
    int filenameLength = memory->readByte (filenameIndex);
    if (filenameLength > 0) {
      const int bufferSize = 0x2000;
      unsigned char* buffer = (unsigned char*) mem_allocate ("char[]", bufferSize, PTYPE_ORDINARY);
      buffer[filenameLength] = 0;
      memory->readBytes (buffer, filenameIndex + 1, filenameLength);
      int bytesToWrite = (registers->bx () << 16) + registers->cx ();
      const int startIndex = filenameIndex + 0x80;
      if (startIndex + bytesToWrite <= 0x110000 && bytesToWrite > -1) {
        int bytesWritten = 0;
        fileOutputStream_t* fos = new fileOutputStream_t ((char*) buffer);
        while (fos->getErrorCode () == 0 && bytesToWrite > 0) {
          int count = bytesToWrite > bufferSize ? bufferSize : bytesToWrite;
          memory->readBytes (buffer, startIndex + bytesWritten, count);
          fos->write ((char*) buffer, count);
          bytesWritten += count;
          bytesToWrite -= count;
        }
        if (fos->getErrorCode () == 0) {
          fos->close ();
        }
        if (fos->getErrorCode () == 0) {
          if (bytesWritten != 1) {
            printf ("%05X bytes were written.\n", bytesWritten);
          } else {
            printf ("00001 byte was written.\n");
          }
        } else {
          printStreamError (fos);
        }
        fos->dump ();
      } else {
        printf ("Can write no more than %X bytes.\n", 0x110000 - startIndex);
      }
      mem_release (buffer);
    } else {
      printf ("No filename specified.\n");
    }
  }

  ___BTPOP;
  return errorCode;
}

void finishOff (void) {
  ___BTPUSH;
  processor->dump ();
  assembler->dump ();
  registers->dump ();
  printer->dump ();
  memory->dump ();
  input->dump ();
  aAddress->dump ();
  dAddress->dump ();
  eAddress->dump ();
  uAddress->dump ();

  objectReleasePool_t::instance ()->dump ();
  mem_printExitStats ();
  ___BTPOP;
}

void init (void) {
  ___BTPUSH;
  errors_printBacktraceFatalSignals ();

  iosystem_t* iosystem = new iosystem_t ();

  memory = new memory_t ();
  input = new lineInputReader_t ();
  processor = new processor_t (memory, iosystem);
  registers = processor->registers ();
  registers->reserve ();
  printer = new printer_t (registers, memory);
  assembler = new assembler_t (memory, input);
  aAddress = new address_t (registers->cs (), registers->ip ());
  dAddress = new address_t (registers->cs (), registers->ip ());
  eAddress = new address_t (registers->cs (), registers->ip ());
  uAddress = new address_t (registers->cs (), registers->ip ());

  filenameIndex = aAddress->value () - 0x80;
  iosystem->addDevice (new realTimeClock_t ());
  iosystem->addDevice (new videoCard_t (memory));
  iosystem->dump ();

  loadFonts ();
  loadInterruptServiceRoutines ();

  ___BTPOP;
}

void loadFonts (void) {
  ___BTPUSH;

  int index = 0xC0000;
  font_t* font = font_t::get8x8Font ();
  memory->writeBytes (index, font->buffer (), font->bufferSize ());
  index += font->bufferSize ();
  font = font_t::get8x14Font ();
  memory->writeBytes (index, font->buffer (), font->bufferSize ());
  index += font->bufferSize ();
  font = font_t::get8x16Font ();
  memory->writeBytes (index, font->buffer (), font->bufferSize ());

  ___BTPOP;
}

int loadInterruptServiceRoutine (int index, const char* filename) {
  ___BTPUSH;

  interruptService_t* service = new interruptService_t (filename);
  if (!service->hasErrors ()) {
    if ((index & 0xF) != 0) {
      index &= 0xFFFFFFF0;
      index += 0x10;
    }
    int intTableIndex = 4 * service->serviceNumber ();
    memory->writeWord (intTableIndex, service->ipStart ());
    memory->writeWord (intTableIndex + 2, service->csStart () + (index >> 4));
    memory->writeBytes (index, service->data (), service->dataSize ());
  }
  service->dump ();

  ___BTPOP;
  return index;
}

void loadInterruptServiceRoutines (void) {
  ___BTPUSH;

  int index = 0xF0000;
  const char* filenames[] = {"servicedev/VideoService.x"};
  const int numberOfFilesToLoad = sizeof (filenames) / sizeof (void*);
  for (int i = 0; i < numberOfFilesToLoad; ++i) {
    index = loadInterruptServiceRoutine (index, filenames[i]);
  }

  ___BTPOP;
}

int noFurtherInputAllowed (int errorCode) {
  ___BTPUSH;

  if (errorCode == 0) {
    int c = input->read ();
    if (c != 0) {
      errorCode = 1;
      if (c != ' ') {
        input->rewind ();
      }
    }
  }

  ___BTPOP;
  return errorCode;
}

/* Ignores the errorCode for the moment. */
void printErrorMessage (int errorCode) {
  ___BTPUSH;

  for (int i = 0; i < input->position () + 1; ++i) {
    putchar (' ');
  }
  printf ("^ Error\n");

  ___BTPOP;
}

void printRegisters (void) {
  ___BTPUSH;

  printer->printRegisters ();
  uAddress->setOffset (registers->ip ());
  uAddress->setSegment (registers->cs ());

  ___BTPOP;
}

void printStreamError (stream_t* stream) {
  ___BTPUSH;

  if (stream->getErrorCode () != 0) {
    const char* description = stream->getErrorDescription ();
    if (description == NULL) {
      description = "Unknown I/O error.";
    }
    printf ("%s\n", description);
  }

  ___BTPOP;
}

void processCommandlineArgs (int argc, char** args) {
  ___BTPUSH;

  if (argc > 1) {
    int filenameLength = strlen (args[1]);
    if (filenameLength < 128) {
      memory->writeBytes (filenameIndex + 1, (unsigned char*) args[1], filenameLength);
      memory->writeByte (filenameIndex, filenameLength);
    }

    fileInputStream_t* fis = new fileInputStream_t (args[1]);
    if (fis->getErrorCode () == 0) {
      const int bufferSize = 0x2000;
      unsigned char* buffer = (unsigned char*) mem_allocate ("char[]", bufferSize, PTYPE_ORDINARY);
      int index = uAddress->value ();
      int bytesRead;

      do {
        bytesRead = fis->read (buffer, bufferSize);
        memory->writeBytes (index, buffer, bytesRead);
        index += bytesRead;
      } while (bytesRead == bufferSize);

      mem_release (buffer);
      bytesRead = index - uAddress->value ();
      if (bytesRead > 0xFFFF) {
        processor->setGeneralPurposeRegister (BX, bytesRead >> 16);
      }
      processor->setGeneralPurposeRegister (CX, bytesRead & 0xFFFF);
    } else {
      printStreamError (fis);
    }
    fis->dump ();
  }

  ___BTPOP;
}

int readAddress (address_t* address, int firstChar) {
  ___BTPUSH;

  int offset;
  int segment = address->segment ();
  int errorCode = 0;
  int c = input->read ();
  bool secondCharS = c == 'S';
  if (secondCharS) {
    switch (firstChar) {
    case 'C': offset = registers->cs (); break;
    case 'D': offset = registers->ds (); break;
    case 'E': offset = registers->es (); break;
    case 'S': offset = registers->ss (); break;
    default: errorCode = 1;
    }
  } else {
    input->rewind (2);
    errorCode = readWord (&offset);
  }

  if (errorCode == 0) {
    c = input->read ();
    if (c == ':') {
      segment = offset;
      errorCode = readWord (&offset);
    } else {
      errorCode = secondCharS;
      input->rewind ();
    }
  }

  if (errorCode == 0) {
    address->setOffset (offset);
    address->setSegment (segment);
  }

  ___BTPOP;
  return errorCode;
}

int readWord (int* dst) {
  ___BTPUSH;

  dst[0] = 0;
  int errorCode = 0;
  int c = input->read ();
  if (isxdigit (c)) {
    while (c == '0') {
      c = input->read ();
    }
    int digitsRead = 0;
    while (isxdigit (c) && digitsRead < 4) {
      dst[0] <<= 4;
      dst[0] |= uchar_hexDigitValue (c);
      c = input->read ();
      ++digitsRead;
    }
    input->rewind ();
    if (isxdigit (c)) {
      errorCode = 1;
    }
  } else {
    input->rewind ();
    errorCode = 1;
  }

  ___BTPOP;
  return errorCode;
}
