/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013, 2014 Gerard Visser.

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
#include <lombok/io/filename.h>
#include <lombok/testing/testUtils.h>
#include <lombok/util/string.h>
#include "filenameTests.h"

#define PRINT_TEST printTestedMethod ("filename_t")

#define TEST_CONCAT(x, y) \
  name1 = filename_t::create (x); \
  name2 = filename_t::create (y); \
  name3 = name1->concat (name2); \
  name4 = concat (name1, name2); \
  verify (name3, name4, __LINE__); \
  name4->dump (); \
  name3->dump (); \
  name2->dump (); \
  name1->dump ()

#define TEST_CREATE(specifiedName, normalizedName) \
  filename = filename_t::create (specifiedName); \
  verify (filename, normalizedName, __LINE__); \
  filename->dump ()

#define TEST_PARENT_DIRECTORY(x) \
  name1 = filename_t::create (x); \
  name2 = name1->parentDirectory (); \
  name3 = name1->concat (relativeParent); \
  verify (name2, name3, __LINE__); \
  name3->dump (); \
  name2->dump (); \
  name1->dump ()

#define TEST_WITH_EXTENSION(filename, extension, expectedResult) \
  name1 = filename_t::create (filename); \
  name2 = name1->withExtension (extension); \
  verify (name2, expectedResult, __LINE__); \
  name2->dump (); \
  name1->dump ()

static filename_t* concat (filename_t* name1, filename_t* name2);
static void verify (filename_t* filename, const char* correctName, int errline);
static void verify (filename_t* filename, filename_t* correctName, int errline);

static void testConcat (void) {
  ___BTPUSH;
  PRINT_TEST;

  filename_t* name1;
  filename_t* name2;
  filename_t* name3 = filename_t::create ("een/twee/drie");
  filename_t* name4 = filename_t::create ("/vier/vijf/zes");
  assert (name3->concat (name4) == NULL);
  name4->dump ();
  name3->dump ();

  TEST_CONCAT ("/", "");
  TEST_CONCAT ("/", ".");
  TEST_CONCAT ("/", "vier");
  TEST_CONCAT ("/", "vier/vijf");
  TEST_CONCAT ("/een", "");
  TEST_CONCAT ("/een", "vier");
  TEST_CONCAT ("/een", "vier/vijf");
  TEST_CONCAT ("/een/twee", "");
  TEST_CONCAT ("/een/twee", "vier");
  TEST_CONCAT ("/een/twee", "vier/vijf");

  TEST_CONCAT ("", "");
  TEST_CONCAT ("", "vier");
  TEST_CONCAT ("", "vier/vijf");
  TEST_CONCAT ("een", "");
  TEST_CONCAT ("een", "vier");
  TEST_CONCAT ("een", "vier/vijf");
  TEST_CONCAT ("een/twee", "");
  TEST_CONCAT ("een/twee", "vier");
  TEST_CONCAT ("een/twee", "vier/vijf");

  TEST_CONCAT ("..", "");
  TEST_CONCAT ("..", "vier");
  TEST_CONCAT ("..", "vier/vijf");
  TEST_CONCAT ("../een", "");
  TEST_CONCAT ("../een", "vier");
  TEST_CONCAT ("../een", "vier/vijf");
  TEST_CONCAT ("../een/twee", "");
  TEST_CONCAT ("../een/twee", "vier");
  TEST_CONCAT ("../een/twee", "vier/vijf");
  TEST_CONCAT ("../..", "");
  TEST_CONCAT ("../..", "vier");
  TEST_CONCAT ("../..", "vier/vijf");
  TEST_CONCAT ("../../een", "");
  TEST_CONCAT ("../../een", "vier");
  TEST_CONCAT ("../../een", "vier/vijf");
  TEST_CONCAT ("../../een/twee", "");
  TEST_CONCAT ("../../een/twee", "vier");
  TEST_CONCAT ("../../een/twee", "vier/vijf");

  TEST_CONCAT ("/", "..");
  TEST_CONCAT ("/", "../vier");
  TEST_CONCAT ("/", "../vier/vijf");
  TEST_CONCAT ("/een", "..");
  TEST_CONCAT ("/een", "../vier");
  TEST_CONCAT ("/een", "../vier/vijf");
  TEST_CONCAT ("/een/twee", "..");
  TEST_CONCAT ("/een/twee", "../vier");
  TEST_CONCAT ("/een/twee", "../vier/vijf");
  TEST_CONCAT ("/een/twee/drie", "..");
  TEST_CONCAT ("/een/twee/drie", "../vier");
  TEST_CONCAT ("/een/twee/drie", "../vier/vijf");
  TEST_CONCAT ("/", "../..");
  TEST_CONCAT ("/", "../../vier");
  TEST_CONCAT ("/", "../../vier/vijf");
  TEST_CONCAT ("/een", "../..");
  TEST_CONCAT ("/een", "../../vier");
  TEST_CONCAT ("/een", "../../vier/vijf");
  TEST_CONCAT ("/een/twee", "../..");
  TEST_CONCAT ("/een/twee", "../../vier");
  TEST_CONCAT ("/een/twee", "../../vier/vijf");
  TEST_CONCAT ("/een/twee/drie", "../..");
  TEST_CONCAT ("/een/twee/drie", "../../vier");
  TEST_CONCAT ("/een/twee/drie", "../../vier/vijf");
  TEST_CONCAT ("/", "../../..");
  TEST_CONCAT ("/", "../../../vier");
  TEST_CONCAT ("/", "../../../vier/vijf");
  TEST_CONCAT ("/een", "../../..");
  TEST_CONCAT ("/een", "../../../vier");
  TEST_CONCAT ("/een", "../../../vier/vijf");
  TEST_CONCAT ("/een/twee", "../../..");
  TEST_CONCAT ("/een/twee", "../../../vier");
  TEST_CONCAT ("/een/twee", "../../../vier/vijf");
  TEST_CONCAT ("/een/twee/drie", "../../..");
  TEST_CONCAT ("/een/twee/drie", "../../../vier");
  TEST_CONCAT ("/een/twee/drie", "../../../vier/vijf");
  TEST_CONCAT ("/", "../../../..");
  TEST_CONCAT ("/", "../../../../vier");
  TEST_CONCAT ("/", "../../../../vier/vijf");
  TEST_CONCAT ("/een", "../../../..");
  TEST_CONCAT ("/een", "../../../../vier");
  TEST_CONCAT ("/een", "../../../../vier/vijf");
  TEST_CONCAT ("/een/twee", "../../../..");
  TEST_CONCAT ("/een/twee", "../../../../vier");
  TEST_CONCAT ("/een/twee", "../../../../vier/vijf");
  TEST_CONCAT ("/een/twee/drie", "../../../..");
  TEST_CONCAT ("/een/twee/drie", "../../../../vier");
  TEST_CONCAT ("/een/twee/drie", "../../../../vier/vijf");

  TEST_CONCAT ("", "..");
  TEST_CONCAT ("", "../vier");
  TEST_CONCAT ("", "../vier/vijf");
  TEST_CONCAT ("een", "..");
  TEST_CONCAT ("een", "../vier");
  TEST_CONCAT ("een", "../vier/vijf");
  TEST_CONCAT ("een/twee", "..");
  TEST_CONCAT ("een/twee", "../vier");
  TEST_CONCAT ("een/twee", "../vier/vijf");
  TEST_CONCAT ("een/twee/drie", "..");
  TEST_CONCAT ("een/twee/drie", "../vier");
  TEST_CONCAT ("een/twee/drie", "../vier/vijf");
  TEST_CONCAT ("", "../..");
  TEST_CONCAT ("", "../../vier");
  TEST_CONCAT ("", "../../vier/vijf");
  TEST_CONCAT ("een", "../..");
  TEST_CONCAT ("een", "../../vier");
  TEST_CONCAT ("een", "../../vier/vijf");
  TEST_CONCAT ("een/twee", "../..");
  TEST_CONCAT ("een/twee", "../../vier");
  TEST_CONCAT ("een/twee", "../../vier/vijf");
  TEST_CONCAT ("een/twee/drie", "../..");
  TEST_CONCAT ("een/twee/drie", "../../vier");
  TEST_CONCAT ("een/twee/drie", "../../vier/vijf");
  TEST_CONCAT ("", "../../..");
  TEST_CONCAT ("", "../../../vier");
  TEST_CONCAT ("", "../../../vier/vijf");
  TEST_CONCAT ("een", "../../..");
  TEST_CONCAT ("een", "../../../vier");
  TEST_CONCAT ("een", "../../../vier/vijf");
  TEST_CONCAT ("een/twee", "../../..");
  TEST_CONCAT ("een/twee", "../../../vier");
  TEST_CONCAT ("een/twee", "../../../vier/vijf");
  TEST_CONCAT ("een/twee/drie", "../../..");
  TEST_CONCAT ("een/twee/drie", "../../../vier");
  TEST_CONCAT ("een/twee/drie", "../../../vier/vijf");
  TEST_CONCAT ("", "../../../..");
  TEST_CONCAT ("", "../../../../vier");
  TEST_CONCAT ("", "../../../../vier/vijf");
  TEST_CONCAT ("een", "../../../..");
  TEST_CONCAT ("een", "../../../../vier");
  TEST_CONCAT ("een", "../../../../vier/vijf");
  TEST_CONCAT ("een/twee", "../../../..");
  TEST_CONCAT ("een/twee", "../../../../vier");
  TEST_CONCAT ("een/twee", "../../../../vier/vijf");
  TEST_CONCAT ("een/twee/drie", "../../../..");
  TEST_CONCAT ("een/twee/drie", "../../../../vier");
  TEST_CONCAT ("een/twee/drie", "../../../../vier/vijf");

  TEST_CONCAT ("..", "..");
  TEST_CONCAT ("..", "../vier");
  TEST_CONCAT ("..", "../vier/vijf");
  TEST_CONCAT ("../een", "..");
  TEST_CONCAT ("../een", "../vier");
  TEST_CONCAT ("../een", "../vier/vijf");
  TEST_CONCAT ("../een/twee", "..");
  TEST_CONCAT ("../een/twee", "../vier");
  TEST_CONCAT ("../een/twee", "../vier/vijf");
  TEST_CONCAT ("../een/twee/drie", "..");
  TEST_CONCAT ("../een/twee/drie", "../vier");
  TEST_CONCAT ("../een/twee/drie", "../vier/vijf");
  TEST_CONCAT ("..", "../..");
  TEST_CONCAT ("..", "../../vier");
  TEST_CONCAT ("..", "../../vier/vijf");
  TEST_CONCAT ("../een", "../..");
  TEST_CONCAT ("../een", "../../vier");
  TEST_CONCAT ("../een", "../../vier/vijf");
  TEST_CONCAT ("../een/twee", "../..");
  TEST_CONCAT ("../een/twee", "../../vier");
  TEST_CONCAT ("../een/twee", "../../vier/vijf");
  TEST_CONCAT ("../een/twee/drie", "../..");
  TEST_CONCAT ("../een/twee/drie", "../../vier");
  TEST_CONCAT ("../een/twee/drie", "../../vier/vijf");
  TEST_CONCAT ("..", "../../..");
  TEST_CONCAT ("..", "../../../vier");
  TEST_CONCAT ("..", "../../../vier/vijf");
  TEST_CONCAT ("../een", "../../..");
  TEST_CONCAT ("../een", "../../../vier");
  TEST_CONCAT ("../een", "../../../vier/vijf");
  TEST_CONCAT ("../een/twee", "../../..");
  TEST_CONCAT ("../een/twee", "../../../vier");
  TEST_CONCAT ("../een/twee", "../../../vier/vijf");
  TEST_CONCAT ("../een/twee/drie", "../../..");
  TEST_CONCAT ("../een/twee/drie", "../../../vier");
  TEST_CONCAT ("../een/twee/drie", "../../../vier/vijf");
  TEST_CONCAT ("..", "../../../..");
  TEST_CONCAT ("..", "../../../../vier");
  TEST_CONCAT ("..", "../../../../vier/vijf");
  TEST_CONCAT ("../een", "../../../..");
  TEST_CONCAT ("../een", "../../../../vier");
  TEST_CONCAT ("../een", "../../../../vier/vijf");
  TEST_CONCAT ("../een/twee", "../../../..");
  TEST_CONCAT ("../een/twee", "../../../../vier");
  TEST_CONCAT ("../een/twee", "../../../../vier/vijf");
  TEST_CONCAT ("../een/twee/drie", "../../../..");
  TEST_CONCAT ("../een/twee/drie", "../../../../vier");
  TEST_CONCAT ("../een/twee/drie", "../../../../vier/vijf");
  TEST_CONCAT ("../..", "..");
  TEST_CONCAT ("../..", "../vier");
  TEST_CONCAT ("../..", "../vier/vijf");
  TEST_CONCAT ("../../een", "..");
  TEST_CONCAT ("../../een", "../vier");
  TEST_CONCAT ("../../een", "../vier/vijf");
  TEST_CONCAT ("../../een/twee", "..");
  TEST_CONCAT ("../../een/twee", "../vier");
  TEST_CONCAT ("../../een/twee", "../vier/vijf");
  TEST_CONCAT ("../../een/twee/drie", "..");
  TEST_CONCAT ("../../een/twee/drie", "../vier");
  TEST_CONCAT ("../../een/twee/drie", "../vier/vijf");
  TEST_CONCAT ("../..", "../..");
  TEST_CONCAT ("../..", "../../vier");
  TEST_CONCAT ("../..", "../../vier/vijf");
  TEST_CONCAT ("../../een", "../..");
  TEST_CONCAT ("../../een", "../../vier");
  TEST_CONCAT ("../../een", "../../vier/vijf");
  TEST_CONCAT ("../../een/twee", "../..");
  TEST_CONCAT ("../../een/twee", "../../vier");
  TEST_CONCAT ("../../een/twee", "../../vier/vijf");
  TEST_CONCAT ("../../een/twee/drie", "../..");
  TEST_CONCAT ("../../een/twee/drie", "../../vier");
  TEST_CONCAT ("../../een/twee/drie", "../../vier/vijf");
  TEST_CONCAT ("../..", "../../..");
  TEST_CONCAT ("../..", "../../../vier");
  TEST_CONCAT ("../..", "../../../vier/vijf");
  TEST_CONCAT ("../../een", "../../..");
  TEST_CONCAT ("../../een", "../../../vier");
  TEST_CONCAT ("../../een", "../../../vier/vijf");
  TEST_CONCAT ("../../een/twee", "../../..");
  TEST_CONCAT ("../../een/twee", "../../../vier");
  TEST_CONCAT ("../../een/twee", "../../../vier/vijf");
  TEST_CONCAT ("../../een/twee/drie", "../../..");
  TEST_CONCAT ("../../een/twee/drie", "../../../vier");
  TEST_CONCAT ("../../een/twee/drie", "../../../vier/vijf");
  TEST_CONCAT ("../..", "../../../..");
  TEST_CONCAT ("../..", "../../../../vier");
  TEST_CONCAT ("../..", "../../../../vier/vijf");
  TEST_CONCAT ("../../een", "../../../..");
  TEST_CONCAT ("../../een", "../../../../vier");
  TEST_CONCAT ("../../een", "../../../../vier/vijf");
  TEST_CONCAT ("../../een/twee", "../../../..");
  TEST_CONCAT ("../../een/twee", "../../../../vier");
  TEST_CONCAT ("../../een/twee", "../../../../vier/vijf");
  TEST_CONCAT ("../../een/twee/drie", "../../../..");
  TEST_CONCAT ("../../een/twee/drie", "../../../../vier");
  TEST_CONCAT ("../../een/twee/drie", "../../../../vier/vijf");

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testCreate (void) {
  ___BTPUSH;
  PRINT_TEST;

  filename_t* filename;

  assert (filename_t::create ((const char*) NULL) == NULL);
  assert (filename_t::create ((charSequence_t*) NULL) == NULL);

  TEST_CREATE ("", ".");
  TEST_CREATE ("/", "/");
  TEST_CREATE ("een/twee/drie", "een/twee/drie");
  TEST_CREATE ("een/twee/drie/", "een/twee/drie");
  TEST_CREATE ("/een/twee/drie", "/een/twee/drie");
  TEST_CREATE ("/een/twee/drie/", "/een/twee/drie");

  TEST_CREATE ("/.", "/");
  TEST_CREATE ("/./", "/");
  TEST_CREATE ("/./een", "/een");
  TEST_CREATE ("/twee/.", "/twee");
  TEST_CREATE ("/twee/./", "/twee");
  TEST_CREATE ("/twee/./een", "/twee/een");

  TEST_CREATE ("/..", "/");
  TEST_CREATE ("/../", "/");
  TEST_CREATE ("/../drie", "/drie");
  TEST_CREATE ("/twee/..", "/");
  TEST_CREATE ("/twee/../", "/");
  TEST_CREATE ("/twee/../drie", "/drie");
  TEST_CREATE ("/een/twee/..", "/een");
  TEST_CREATE ("/een/twee/../", "/een");
  TEST_CREATE ("/een/twee/../drie", "/een/drie");

  TEST_CREATE (".", ".");
  TEST_CREATE ("./", ".");
  TEST_CREATE ("./een", "een");
  TEST_CREATE ("twee/.", "twee");
  TEST_CREATE ("twee/./", "twee");
  TEST_CREATE ("twee/./een", "twee/een");

  TEST_CREATE ("..", "..");
  TEST_CREATE ("../", "..");
  TEST_CREATE ("../drie", "../drie");
  TEST_CREATE ("../..", "../..");
  TEST_CREATE ("../../", "../..");
  TEST_CREATE ("../../drie", "../../drie");
  TEST_CREATE ("twee/..", ".");
  TEST_CREATE ("twee/../", ".");
  TEST_CREATE ("twee/../drie", "drie");
  TEST_CREATE ("een/twee/..", "een");
  TEST_CREATE ("een/twee/../", "een");
  TEST_CREATE ("een/twee/../drie", "een/drie");
  TEST_CREATE ("../twee/..", "..");
  TEST_CREATE ("../twee/../", "..");
  TEST_CREATE ("../twee/../drie", "../drie");
  TEST_CREATE ("../twee/../..", "../..");

  string_t* name = new string_t ("..//./abcd/../.\340\352\360/...////..", CHAR_ENC_ISO8859_2);
  TEST_CREATE (name, "../.\305\225\304\231\304\221");
  name->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testEquals (void) {
  ___BTPUSH;
  PRINT_TEST;

  filename_t* name1 = filename_t::create ("een/twee/drie");
  assert (name1->equals (name1));
  filename_t* name2 = filename_t::create ("een//vier/./../twee/drie");
  assert (name1->equals (name2));
  assert (name2->equals (name1));
  name2->dump ();
  name2 = filename_t::create ("/een/twee/drie");
  assert (!name1->equals (name2));
  assert (!name2->equals (name1));
  name2->dump ();
  name1->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testHashCode (void) {
  ___BTPUSH;
  PRINT_TEST;

  filename_t* name1 = filename_t::create ("een/twee/drie");
  filename_t* name2 = filename_t::create ("een//vier/./../twee/drie");
  assert (name1->hashCode () == name2->hashCode ());
  name2->dump ();
  name1->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testIsAbsolute (void) {
  ___BTPUSH;
  PRINT_TEST;

  filename_t* name = filename_t::create ("/");
  assert (name->isAbsolute ());
  name->dump ();
  name = filename_t::create ("/a");
  assert (name->isAbsolute ());
  name->dump ();
  name = filename_t::create ("a");
  assert (!name->isAbsolute ());
  name->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testIsReflexive (void) {
  ___BTPUSH;
  PRINT_TEST;

  filename_t* name = filename_t::create (".");
  assert (name->isReflexive ());
  name->dump ();
  name = filename_t::create ("..");
  assert (!name->isReflexive ());
  name->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testIsRoot (void) {
  ___BTPUSH;
  PRINT_TEST;

  filename_t* name = filename_t::create ("/");
  assert (name->isRoot ());
  name->dump ();
  name = filename_t::create ("/a");
  assert (!name->isRoot ());
  name->dump ();
  name = filename_t::create ("a");
  assert (!name->isRoot ());
  name->dump ();

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testParentDirectory (void) {
  ___BTPUSH;
  PRINT_TEST;

  filename_t* name1;
  filename_t* name2;
  filename_t* name3;
  filename_t* relativeParent = filename_t::create ("..");

  TEST_PARENT_DIRECTORY ("/");
  TEST_PARENT_DIRECTORY (".");
  TEST_PARENT_DIRECTORY ("..");
  TEST_PARENT_DIRECTORY ("/een");
  TEST_PARENT_DIRECTORY ("/een/twee");
  TEST_PARENT_DIRECTORY ("/een/twee/drie");
  TEST_PARENT_DIRECTORY ("een");
  TEST_PARENT_DIRECTORY ("een/twee");
  TEST_PARENT_DIRECTORY ("een/twee/drie");
  TEST_PARENT_DIRECTORY ("../een");
  TEST_PARENT_DIRECTORY ("../../een/twee");
  TEST_PARENT_DIRECTORY ("../../..");

  relativeParent->dump ();
  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

static void testWithExtension (void) {
  ___BTPUSH;
  PRINT_TEST;

  filename_t* name1;
  filename_t* name2;

  TEST_WITH_EXTENSION ("naam.h", "cpp", "naam.cpp");
  TEST_WITH_EXTENSION ("/water.spin", "plant", "/water.plant");
  TEST_WITH_EXTENSION (".", "blad", ".blad");
  TEST_WITH_EXTENSION (".", "", ".");
  TEST_WITH_EXTENSION ("/", "d", "/.d");
  TEST_WITH_EXTENSION ("/", "", "/");
  TEST_WITH_EXTENSION ("..", "bad", "..bad");
  TEST_WITH_EXTENSION ("../..", "kamer", "../..kamer");
  TEST_WITH_EXTENSION ("..", "", "..");
  TEST_WITH_EXTENSION ("../..", "", "../..");
  TEST_WITH_EXTENSION ("../nepparterretrappen", "f", "../nepparterretrappen.f");
  TEST_WITH_EXTENSION ("/onderzo.ek/meetsysteem.doos", "b", "/onderzo.ek/meetsysteem.b");
  TEST_WITH_EXTENSION ("tuin/huis/lego.pop", "vogel", "tuin/huis/lego.vogel");
  TEST_WITH_EXTENSION ("was/it/a.car/or.a.cat.i.heard", "saw", "was/it/a.car/or.a.cat.i.saw");

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

const struct filenameTestsStruct filenameTests = {
  testCreate,
  testConcat,
  testEquals,
  testHashCode,
  testIsAbsolute,
  testIsReflexive,
  testIsRoot,
  testParentDirectory,
  testWithExtension
};

typedef struct {
  int* indices;
  int indexCount;
  int parentDirs;
} nameData_t;

static void cleanupNameData (nameData_t* nameData) {
  ___BTPUSH;
  mem_release (nameData->indices);
  ___BTPOP;
}

static filename_t* concat (filename_t* name1, filename_t* name2) {
  ___BTPUSH;

  char buf[1024];
  stpcpy (stpcpy (stpcpy (buf, name1->get ()), "/"), name2->get ());
  filename_t* result = filename_t::create (buf);

  ___BTPOP;
  return result;
}

static void determineIndexCount (nameData_t* nameData, const char* name, int offset) {
  ___BTPUSH;

  nameData->indexCount = 1;
  while (name[offset] != 0) {
    if (name[offset] == '/') {
      ++nameData->indexCount;
    }
    ++offset;
  }

  ___BTPOP;
}

static void determineIndices (nameData_t* nameData, const char* name, int offset) {
  ___BTPUSH;

  int i = 0;
  nameData->indices = (int*) mem_allocate ("int[]", sizeof (int) * nameData->indexCount, PTYPE_ORDINARY);
  while (name[offset] != 0) {
    if (name[offset] == '/') {
      nameData->indices[i] = offset;
      ++i;
    }
    ++offset;
  }
  nameData->indices[i] = offset;

  ___BTPOP;
}

static int determineParentDirs (nameData_t* nameData, const char* name) {
  ___BTPUSH;

  int offset = 0;
  nameData->parentDirs = 0;
  while (name[offset] == '.' && name[offset + 1] == '.' && name[offset + 2] == '/') {
    ++nameData->parentDirs;
    offset += 3;
  }
  if (name[offset] == '.' && name[offset + 1] == '.' && name[offset + 2] == 0) {
    ++nameData->parentDirs;
    offset += 2;
  } else if (offset > 0) {
    --offset;
  } else if (name[0] == '/') {
    ++offset;
  }

  ___BTPOP;
  return offset;
}

static void initNameData (nameData_t* nameData, const char* name) {
  ___BTPUSH;

  int offset = determineParentDirs (nameData, name);
  determineIndexCount (nameData, name, offset);
  determineIndices (nameData, name, offset);

  ___BTPOP;
}

#ifdef DEBUG_MODE
# define ASSERT(expression, errline) \
  if (!(expression)) errors_printMessageAndExit ("%s:%d: Assertion `%s' failed.\nError in line %d.\n", __FILE__, __LINE__, #expression, errline)
#else
# define ASSERT(expression, errline) assert (expression)
#endif

static void verify (filename_t* filename, const char* correctName, int errline) {
  ___BTPUSH;

  ASSERT (strcmp (filename->get (), correctName) == 0, errline);

#ifdef DEBUG_MODE

  nameData_t nameData;
  initNameData (&nameData, correctName);
  ASSERT (filename->m_parentDirs == nameData.parentDirs, errline);
  ASSERT (filename->m_indexCount == nameData.indexCount, errline);
  for (int i = 0; i < nameData.indexCount; ++i) {
    ASSERT (filename->m_indices[i] == nameData.indices[i], errline);
  }
  cleanupNameData (&nameData);

#endif

  ___BTPOP;
}

static void verify (filename_t* filename, filename_t* correctName, int errline) {
  ___BTPUSH;

  ASSERT (strcmp (filename->get (), correctName->get ()) == 0, errline);

#ifdef DEBUG_MODE

  ASSERT (filename->m_parentDirs == correctName->m_parentDirs, errline);
  ASSERT (filename->m_indexCount == correctName->m_indexCount, errline);
  for (int i = 0; i < correctName->m_indexCount; ++i) {
    ASSERT (filename->m_indices[i] == correctName->m_indices[i], errline);
  }

#endif

  ___BTPOP;
}


/*
 {
  ___BTPUSH;
  PRINT_TEST;



  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

 */
