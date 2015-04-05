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

#include <lombok/base/all.h>
#include <lombok/io/all.h>
#include <lombok/util/all.h>
#include "asmErrors.h"
#include "asmPreparator.h"
#include "asmTokeniser.h"
#include "compiler.h"
#include "programmeBuilder.h"
#include "programmeLogger.h"

filename_t* inputFile = NULL;
int interruptServiceNumber = 0xFFFF;
bool logfileToBeCreated = false;

void createLogfile (programme_t* programme);
asmWriter_t* getOutputWriter (void);
pushbackStream_t* init (int argc, char** args);
void processCommandlineArgs (int argc, char** args);
int readHexNumber (const char* str);

int main (int argc, char** args, char** env) {
  ___BTPUSH;
  errors_printBacktraceFatalSignals ();

  pushbackStream_t* instream = init (argc, args);
  asmTokeniser_t* tokeniser = new asmTokeniser_t ();
  arrayList_t* tokens = tokeniser->getTokens (instream);
  tokeniser->dump ();
  instream->dump ();
  asmPreparator_t* preparator = new asmPreparator_t ();
  arrayList_t* preparedTokens = preparator->prepareTokens (tokens);
  preparator->dump ();
  tokens->dump ();
  programmeBuilder_t* programmeBuilder = new programmeBuilder_t ();
  programme_t* programme = programmeBuilder->build (preparedTokens);
  programmeBuilder->dump ();
  preparedTokens->dump ();
  compiler_t* compiler = new compiler_t ();
  asmWriter_t* writer = getOutputWriter ();
  compiler->compile (writer, programme, interruptServiceNumber);
  compiler->dump ();
  writer->dump ();
  if (logfileToBeCreated) {
    createLogfile (programme);
  }
  programme->dump ();
  inputFile->dump ();

  objectReleasePool_t::instance ()->dump ();
  mem_printExitStats ();
  ___BTPOP;
  return 0;
}

void createLogfile (programme_t* programme)  {
  ___BTPUSH;

  filename_t* logfile = inputFile->withExtension ("html");
  fileOutputStream_t* fos = new fileOutputStream_t (logfile->get ());
  programmeLogger_write (fos, programme);
  logfile->dump ();
  fos->dump ();

  ___BTPOP;
}

asmWriter_t* getOutputWriter (void)  {
  ___BTPUSH;

  filename_t* outputFile = inputFile->withExtension ("x");
  asmWriter_t* writer = new asmWriter_t (new fileOutputStream_t (outputFile->get ()));
  outputFile->dump ();

  ___BTPOP;
  return writer;
}

pushbackStream_t* init (int argc, char** args) {
  ___BTPUSH;

  pushbackStream_t* fileToCompile;
  processCommandlineArgs (argc, args);
  if (inputFile != NULL) {
    fileToCompile = new pushbackStream_t (new fileInputStream_t (inputFile->get ()));
    if (fileToCompile->getErrorCode () != 0) {
      asmErrors::exit (fileToCompile->getErrorDescription ());
    }
    asmErrors::init (inputFile->get ());
  } else {
    asmErrors::exit ("no input files");
  }

  ___BTPOP;
  return fileToCompile;
}

void processCommandlineArgs (int argc, char** args) {
  ___BTPUSH;

  int i = 1;
  char message[256];
  while (i < argc) {
    if (args[i][0] == '-') {
      switch (args[i][1]) {
      case 'l':
        logfileToBeCreated = true;
        break;

      case 's': {
        unsigned int number = readHexNumber (args[i] + 2);
        if (number > 0xFF) {
          asmErrors::exit ("interrupt numbers range from 0 to FF");
        }
        interruptServiceNumber = number;
      } break;

      default:
        sprintf (message, "unknown option %c", args[i][1]);
        asmErrors::exit (message);
      }
    } else if (inputFile != NULL) {
      asmErrors::exit ("only one input file allowed");
    } else {
      inputFile = filename_t::create (args[i]);
    }
    ++i;
  }

  ___BTPOP;
}

int readHexNumber (const char* str) {
  ___BTPUSH;

  int i = 0;
  int result = 0;
  while (uchar_isHexDigit (str[i])) {
    result <<= 4;
    result |= uchar_hexDigitValue (str[i]);
    ++i;
  }
  if (i == 0 || str[i] != 0) {
    char message[512];
    sprintf (message, "\342\200\230%s\342\200\231 is not a nonnegative integer", str);
    asmErrors::exit (message);
  }

  ___BTPOP;
  return result;
}
