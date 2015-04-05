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

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <lombok/base/mem.h>
#include <lombok/base/errors.h>
#include <lombok/base/hashCode.h>
#include <lombok/io/filename.h>

REFL_IMPL (filename_t, object_t)

/*
   The positions of some characters in the (normalized) name are stored in an
   array called m_indices; the length of the name is also stored in that array.

   *) Rule.
      Each `/' (slash) gets an index, except if it is
       1) followed by `../' or `..\0' (where `\0' denotes the terminating
          null-character);
       2) the first character of the name.

   *) Examples.
      m_name:                m_indices:        m_parentDirs:
      "."                    {1}               0
      ".."                   {2}               1
      "een"                  {3}               0
      "een/twee/drie"        {3, 8, 13}        0
      "../../../.."          {11}              4
      "../../een/twee"       {5, 9, 14}        2
      "/"                    {1}               0
      "/een"                 {4}               0
      "/een/twee"            {4, 9}            0
 */

#define SUBSTRING(to, from, len) \
  memcpy (to, from, len); \
  to[len] = 0

typedef struct {
  char* name;
  int* indices;
  int indexCount;
  int parentDirs;
  int nameLength;
  int index;
} nameData_t;

static void addIndices (int* to, const int* from, const int number, const int indexCorrection);
static void allocateNameAndIndices (char** name, int nameLength, int** indices, int indexCount);
static void initNameData (nameData_t* nameData, const char* name);

filename_t::filename_t (const char* name, const int* indices, int indexCount, int parentDirs) :
  m_name (name), m_indices (indices), m_indexCount (indexCount), m_parentDirs (parentDirs) {
}

filename_t::~filename_t (void) {
  ___CBTPUSH;

  mem_release ((void*) m_name);
  /* The m_indices are in the same buffer as m_name and should not be released separately. */

  ___CBTPOP;
}

filename_t* filename_t::concat (filename_t* other) {
  ___CBTPUSH;

  char* name;
  int* indices;
  filename_t* result;

  if (other->isAbsolute ()) {

    result = NULL;

  } else if (other->isReflexive ()) {

    result = this;
    reserve ();

  } else if (isReflexive ()) {

    result = other;
    other->reserve ();

  } else if (other->m_parentDirs == 0) {

    const int otherLen = other->m_indices[other->m_indexCount - 1];
    if (isRoot ()) {
      allocateNameAndIndices (&name, 1 + otherLen, &indices, other->m_indexCount);
      memcpy (mempcpy (name, "/", 1), other->m_name, otherLen + 1);
      addIndices (indices, other->m_indices, other->m_indexCount, 1);
      result = new filename_t (name, indices, other->m_indexCount, m_parentDirs);
    } else {
      const int thisLen = m_indices[m_indexCount - 1];
      allocateNameAndIndices (&name, thisLen + otherLen + 1, &indices, m_indexCount + other->m_indexCount);
      memcpy (mempcpy (mempcpy (name, m_name, thisLen), "/", 1), other->m_name, otherLen + 1);
      memcpy (indices, m_indices, sizeof (int) * m_indexCount);
      addIndices (indices + m_indexCount, other->m_indices, other->m_indexCount, thisLen + 1);
      result = new filename_t (name, indices, m_indexCount + other->m_indexCount, m_parentDirs);
    }

  } else if (m_indexCount - (m_parentDirs != 0) > other->m_parentDirs) {

    const int otherIndexCount = other->m_indexCount - 1;
    const int thisIndexCount = m_indexCount - other->m_parentDirs;
    const int thisLen = m_indices[thisIndexCount - 1];
    const int otherLen = other->m_indices[otherIndexCount] - other->m_indices[0];
    allocateNameAndIndices (&name, thisLen + otherLen, &indices, thisIndexCount + otherIndexCount);
    memcpy (mempcpy (name, m_name, thisLen), other->m_name + other->m_indices[0], otherLen + 1);
    memcpy (indices, m_indices, sizeof (int) * thisIndexCount);
    addIndices (indices + thisIndexCount, other->m_indices + 1, otherIndexCount, thisLen - other->m_indices[0]);
    result = new filename_t (name, indices, thisIndexCount + otherIndexCount, m_parentDirs);

  } else if (isAbsolute ()) {

    if (other->m_indexCount > 1) {
      const int otherIndexCount = other->m_indexCount - 1;
      const int otherLen = other->m_indices[otherIndexCount] - other->m_indices[0];
      allocateNameAndIndices (&name, otherLen, &indices, otherIndexCount);
      memcpy (name, other->m_name + other->m_indices[0], otherLen + 1);
      addIndices (indices, other->m_indices + 1, otherIndexCount, -other->m_indices[0]);
      result = new filename_t (name, indices, otherIndexCount, 0);
    } else {
      allocateNameAndIndices (&name, 1, &indices, 1);
      memcpy (name, "/", 2);
      indices[0] = 1;
      result = new filename_t (name, indices, 1, 0);
    }

  } else if (m_parentDirs == 0) {

    const int parentDirs = other->m_parentDirs - m_indexCount;
    if (parentDirs > 0 || other->m_indexCount > 1) {
      const int indexCount = other->m_indexCount - (parentDirs == 0);
      const int nameOffset = other->m_indices[0] + 1 - 3 * parentDirs;
      const int nameLen = other->m_indices[other->m_indexCount - 1] - nameOffset;
      allocateNameAndIndices (&name, nameLen, &indices, indexCount);
      memcpy (name, other->m_name + nameOffset, nameLen + 1);
      addIndices (indices, other->m_indices + (parentDirs == 0), indexCount, -nameOffset);
      result = new filename_t (name, indices, indexCount, parentDirs);
    } else {
      allocateNameAndIndices (&name, 1, &indices, 1);
      memcpy (name, ".", 2);
      indices[0] = 1;
      result = new filename_t (name, indices, 1, 0);
    }

  } else {

    const int extraParentDirs = other->m_parentDirs - m_indexCount + 1;
    if (extraParentDirs > 0 || other->m_indexCount > 1) {
      const int indexCount = other->m_indexCount;
      const int nameOffset = other->m_indices[0] + 1 - 3 * extraParentDirs;
      const int nameLen = other->m_indices[other->m_indexCount - 1] - nameOffset;
      allocateNameAndIndices (&name, nameLen + 3 * m_parentDirs, &indices, indexCount);
      memcpy (mempcpy (name, m_name, 3 * m_parentDirs), other->m_name + nameOffset, nameLen + 1);
      name[3 * m_parentDirs - 1] = '/';
      addIndices (indices, other->m_indices, indexCount, 3 * m_parentDirs - nameOffset);
      result = new filename_t (name, indices, indexCount, m_parentDirs + extraParentDirs);
    } else {
      allocateNameAndIndices (&name, 3 * m_parentDirs - 1, &indices, 1);
      mempcpy (name, m_name, 3 * m_parentDirs - 1);
      indices[0] = 3 * m_parentDirs - 1;
      name[3 * m_parentDirs - 1] = 0;
      result = new filename_t (name, indices, 1, m_parentDirs);
    }

  }

  ___CBTPOP;
  return result;
}

filename_t* filename_t::create (const char* filename) {
  ___CBTPUSH;

  filename_t* result = NULL;
  if (filename != NULL) {
    nameData_t nameData;
    initNameData (&nameData, filename);
    result = new filename_t (nameData.name, nameData.indices, nameData.indexCount, nameData.parentDirs);
  }

  ___CBTPOP;
  return result;
}

filename_t* filename_t::create (charSequence_t* filename) {
  ___CBTPUSH;

  filename_t* result = NULL;
  if (filename != NULL) {
    const int bufSize = filename->length () + 1;
    char buf[bufSize];
    char* utf8 = filename->toUtf8 (buf, bufSize);
    result = filename_t::create (utf8);
    if (utf8 != buf) {
      mem_release (utf8);
    }
  }

  ___CBTPOP;
  return result;
}

bool filename_t::equals (object_t* other) {
  ___CBTPUSH;

  bool result = this == other;
  if (!result) {
    result = other->getClass () == classof (filename_t) && strcmp (((filename_t*) other)->m_name, m_name) == 0;
  }

  ___CBTPOP;
  return result;
}

const char* filename_t::get (void) {
  ___CBTPUSH;

  const char* result = m_name;

  ___CBTPOP;
  return result;
}

unsigned int filename_t::hashCode (void) {
  ___CBTPUSH;

  int hash = m_indices[m_indexCount - 1];
  int i = hash - 16;
  if (i < 0) {
    i = 0;
  }
  int j = 0;
  while (m_name[i] != 0) {
    hash += (unsigned char) m_name[i] << j;
    ++i;
    ++j;
  }

  ___CBTPOP;
  return hashCode_get (hash);
}

bool filename_t::isAbsolute (void) {
  ___CBTPUSH;

  bool result = m_name[0] == '/';

  ___CBTPOP;
  return result;
}

bool filename_t::isReflexive (void) {
  ___CBTPUSH;

  bool result = m_name[0] == '.' && m_name[1] == 0;

  ___CBTPOP;
  return result;
}

bool filename_t::isRoot (void) {
  ___CBTPUSH;

  bool result = m_name[0] == '/' && m_name[1] == 0;

  ___CBTPOP;
  return result;
}

filename_t* filename_t::parentDirectory (void) {
  ___CBTPUSH;

  char* name;
  int namelen;
  int* indices;
  filename_t* result;

  if (isRoot ()) { /* / */

    result = this;
    reserve ();

  } else if (isReflexive ()) { /* . */

    namelen = 2;
    allocateNameAndIndices (&name, namelen, &indices, 1);
    strcpy (name, "..");
    indices[0] = namelen;
    result = new filename_t (name, indices, 1, 1);

  } else if (m_indexCount > 1) { /* /a/b, a/b, ../a */

    namelen = m_indices[m_indexCount - 2];
    allocateNameAndIndices (&name, namelen, &indices, m_indexCount - 1);
    SUBSTRING (name, m_name, namelen);
    memcpy (indices, m_indices, sizeof (int) * (m_indexCount - 1));
    result = new filename_t (name, indices, m_indexCount - 1, m_parentDirs);

  } else if (isAbsolute ()) { /* /a */

    namelen = 1;
    allocateNameAndIndices (&name, namelen, &indices, 1);
    strcpy (name, "/");
    indices[0] = namelen;
    result = new filename_t (name, indices, 1, 0);

  } else if (m_parentDirs > 0) { /* .. */

    namelen = m_indices[0] + 3;
    allocateNameAndIndices (&name, namelen, &indices, 1);
    stpcpy (stpcpy (name, m_name), "/..");
    indices[0] = namelen;
    result = new filename_t (name, indices, 1, m_parentDirs + 1);

  } else { /* a */

    namelen = 1;
    allocateNameAndIndices (&name, namelen, &indices, 1);
    strcpy (name, ".");
    indices[0] = namelen;
    result = new filename_t (name, indices, 1, 0);

  }

  ___CBTPOP;
  return result;
}

filename_t* filename_t::withExtension (const char* extension) {
  ___CBTPUSH;

  char* name;
  int namelen;
  int* indices;
  filename_t* result;
  const int extensionLength = strlen (extension);

  if (m_indexCount > 1) { /* /a/b, a/b, ../a */

    const char* baseEnd = strrchr (m_name + m_indices[m_indexCount - 2] + 1, '.');
    if (baseEnd == NULL) {
      baseEnd = m_name + m_indices[m_indexCount - 1];
    }
    namelen = baseEnd - m_name + extensionLength + 1;
    allocateNameAndIndices (&name, namelen, &indices, m_indexCount);
    stpcpy (stpcpy ((char*) mempcpy (name, m_name, baseEnd - m_name), "."), extension);
    memcpy (indices, m_indices, sizeof (int) * (m_indexCount - 1));
    indices[m_indexCount - 1] = namelen;
    result = new filename_t (name, indices, m_indexCount, m_parentDirs);

  } else if (m_parentDirs > 0) { /* .., ../.. */

    if (extensionLength > 0) {
      int indexCount = 1 + (m_parentDirs > 1);
      namelen = m_indices[m_indexCount - 1] + extensionLength;
      allocateNameAndIndices (&name, namelen, &indices, indexCount);
      stpcpy (stpcpy (name, m_name), extension);
      indices[indexCount - 1] = namelen;
      if (indexCount > 1) {
        indices[0] = m_indices[m_indexCount - 1] - 3;
      }
      result = new filename_t (name, indices, indexCount, m_parentDirs - 1);
    } else {
      result = this;
      reserve ();
    }

  } else if (extensionLength == 0 && isRoot ()) {

    result = this;
    reserve ();

  } else { /* ., a, /, /a */

    const char* baseEnd = strrchr (m_name, '.');
    if (baseEnd == NULL) {
      baseEnd = m_name + m_indices[0];
    }
    namelen = baseEnd - m_name + extensionLength + 1;
    allocateNameAndIndices (&name, namelen, &indices, 1);
    stpcpy (stpcpy ((char*) mempcpy (name, m_name, baseEnd - m_name), "."), extension);
    indices[0] = namelen;
    result = new filename_t (name, indices, 1, 0);

  }

  ___CBTPOP;
  return result;
}

filename_t* filename_t::workingDirectory (void) {
  ___CBTPUSH;

#define INITIAL_SIZE 256

  char* wd;
  char buf[INITIAL_SIZE];
  wd = getcwd (buf, INITIAL_SIZE);
  if (wd == NULL && errno == ERANGE) {
    int size = INITIAL_SIZE << 1;
    char* buf2 = (char*) mem_allocate ("char[]", size, PTYPE_ORDINARY);
    wd = getcwd (buf2, size);
    while (wd == NULL && errno == ERANGE) {
      mem_release (buf2);
      size <<= 1;
      buf2 = (char*) mem_allocate ("char[]", size, PTYPE_ORDINARY);
      wd = getcwd (buf2, size);
    }
    if (wd == NULL) {
      mem_release (buf2);
    }
  }

  filename_t* result = filename_t::create (wd);

  if (!(wd == buf || wd == NULL)) {
    mem_release (wd);
  }

  ___CBTPOP;
  return result;
}

#define DOT    1
#define DOTDOT 2
#define EMPTY  3

static void normalizeAbsoluteName (nameData_t* d);
static void normalizeRelativeName (nameData_t* d);

static void addIndices (int* to, const int* from, const int number, const int indexCorrection) {
  ___BTPUSH;

  for (int i = 0; i < number; ++i) {
    to[i] = from[i] + indexCorrection;
  }

  ___BTPOP;
}

static void allocateNameAndIndices (char** name, int nameLength, int** indices, int indexCount) {
  ___BTPUSH;

  int nameSize = nameLength + 1;
  int r = nameSize % sizeof (long);
  if (r > 0) {
    r = sizeof (long) - r;
  }
  nameSize += r;

  int indicesSize = sizeof (int) * indexCount;
  r = indicesSize % sizeof (long);
  if (r > 0) {
    r = sizeof (long) - r;
  }
  indicesSize += r;

  name[0] = (char*) mem_allocate ("char[]", nameSize + indicesSize, PTYPE_INTERNAL);
  indices[0] = (int*) (name[0] + nameSize);

  ___BTPOP;
}

static int getMaxIndexCount (const char* str) {
  ___BTPUSH;

  int i = 0;
  int result = 1;
  while (str[i] != 0) {
    if (str[i] == '/') {
      ++result;
    }
    ++i;
  }

  ___BTPOP;
  return result;
}

static int identifyFilenamePart (const char* partStart) {
  ___BTPUSH;

  int result = 0;
  if (partStart[0] == '.') {
    if (partStart[1] == '.') {
      if (partStart[2] == '/' || partStart[2] == 0) {
        result = DOTDOT;
      }
    } else if (partStart[1] == '/' || partStart[1] == 0) {
      result = DOT;
    }
  } else if (partStart[0] == '/') {
    result = EMPTY;
  }

  ___BTPOP;
  return result;
}

static void initNameData (nameData_t* nameData, const char* name) {
  ___BTPUSH;

  nameData->parentDirs = 0;
  nameData->indexCount = 0;
  nameData->nameLength = strlen (name);
  allocateNameAndIndices (&nameData->name, nameData->nameLength, &nameData->indices, getMaxIndexCount (name));
  strcpy (nameData->name, name);

  if (name[0] == '/') {
    nameData->index = 1;
    normalizeAbsoluteName (nameData);
  } else {
    nameData->index = 0;
    normalizeRelativeName (nameData);
  }

  ___BTPOP;
}

#define shrinkName(nameData, toIdx, fromIdx) \
  memmove (nameData->name + toIdx, nameData->name + fromIdx, nameData->nameLength - (fromIdx) + 1); \
  nameData->nameLength += toIdx - (fromIdx)

static void normalizeAbsoluteName (nameData_t* d) {
  ___BTPUSH;

  while (d->name[d->index] != 0) {
    int nextFileNamePart = identifyFilenamePart (d->name + d->index);
    switch (nextFileNamePart) {
    case DOT:
      if (d->name[d->index + 1] != 0) {
        shrinkName (d, d->index - 1, d->index + 1);
      } else {
        d->name[d->index] = 0;
      }
      break;

    case DOTDOT: {
      bool zeroAfterDots = d->name[d->index + 2] == 0;
      if (d->indexCount > 1) {

        shrinkName (d, d->indices[d->indexCount - 2], d->index + 2);
        d->index = d->indices[d->indexCount - 2];
        d->indexCount -= zeroAfterDots + 1;
        d->index += !zeroAfterDots;

      } else {

        if (zeroAfterDots) {
          d->name[1] = 0;
        } else {
          shrinkName (d, 0, d->index + 2);
        }
        d->indexCount = 0;
        d->index = 1;

      }
    } break;

    case EMPTY:
      shrinkName (d, d->index, d->index + 1);
      break;

    default:
      while (!(d->name[d->index] == 0 || d->name[d->index] == '/')) {
        ++d->index;
      }
      if (d->name[d->index] != 0) {
        d->indices[d->indexCount] = d->index;
        ++d->indexCount;
        ++d->index;
      }
    }
  }
  if (d->index > 1 && d->name[d->index - 1] == '/') {
    d->name[d->index - 1] = 0;
  } else {
    d->indices[d->indexCount] = d->index;
    ++d->indexCount;
  }

  ___BTPOP;
}

static void normalizeRelativeName (nameData_t* d) {
  ___BTPUSH;

  while (d->name[d->index] != 0) {
    int nextFileNamePart = identifyFilenamePart (d->name + d->index);
    switch (nextFileNamePart) {
    case DOT:
      if (d->index > 0) {
        shrinkName (d, d->index - 1, d->index + 1);
        if (d->name[d->index - 1] == 0) {
          d->indexCount -= d->indexCount > 0;
          --d->index;
        }
      } else if (d->nameLength > 1) {
        shrinkName (d, 0, 2);
      } else {
        ++d->index;
      }
      break;

    case DOTDOT: {
      bool zeroAfterDots = d->name[d->index + 2] == 0;
      if (d->indexCount > 1) {

        shrinkName (d, d->indices[d->indexCount - 2], d->index + 2);
        d->index = d->indices[d->indexCount - 2] + !zeroAfterDots;
        d->indexCount -= (zeroAfterDots || 3 * d->parentDirs == d->index) + 1;

      } else if (d->indexCount == 0) {

        ++d->parentDirs;
        d->index += 2 + !zeroAfterDots;

      } else if (zeroAfterDots) {

        d->indexCount = 0;
        d->name[0] = '.';
        d->name[1] = 0;
        d->index = 1;

      } else {

        d->indexCount = 0;
        shrinkName (d, 0, d->index + 3);
        d->index = 0;

      }
    } break;

    case EMPTY:
      shrinkName (d, d->index, d->index + 1);
      break;

    default:
      if (d->index > 0 && 3 * d->parentDirs == d->index) {
        d->indices[d->indexCount] = d->index - 1;
        ++d->indexCount;
      }
      while (!(d->name[d->index] == 0 || d->name[d->index] == '/')) {
        ++d->index;
      }
      if (d->name[d->index] != 0) {
        d->indices[d->indexCount] = d->index;
        ++d->indexCount;
        ++d->index;
      }
    }
  }
  if (d->index == 0) {
    d->name[0] = '.';
    d->name[1] = 0;
    d->index = 1;
  } else if (d->name[d->index - 1] == '/') {
    d->indexCount -= d->indexCount > 0;
    --d->index;
    d->name[d->index] = 0;
  }
  d->indices[d->indexCount] = d->index;
  ++d->indexCount;

  ___BTPOP;
}
