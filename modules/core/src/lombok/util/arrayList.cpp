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
#include "arrayListIterator.h"

REFL_IMPL (arrayList_t, list_t)

#ifdef DEBUG_MODE

# define DBG_INDEX_CHECK(index) \
  if (index < 0  ||  index > _length - 1) { \
    errors_printMessageAndExit ("%s out of bounds", #index); \
  }

# define DBG_INDEX_CHECK_FOR_INSERT(index) \
  if (index < 0  ||  index > _length) { \
    errors_printMessageAndExit ("%s out of bounds", #index); \
  }

#else

# define DBG_INDEX_CHECK(index)
# define DBG_INDEX_CHECK_FOR_INSERT(index)

#endif

arrayList_t::arrayList_t (int initialCapacity) {
  ___CBTPUSH;

  if (initialCapacity < 2) {
    initialCapacity = 8;
  }
  _buf = (object_t**) mem_allocate ("object_t*[]", sizeof (void*) * initialCapacity, PTYPE_INTERNAL);
  _capacityQuantum = initialCapacity;
  _capacity = initialCapacity;
  _length = 0;

  ___CBTPOP;
}

arrayList_t::~arrayList_t (void) {
  ___CBTPUSH;

  for (int i=0; i<_length; ++i) {
    _buf[i]->dump ();
  }
  mem_release (_buf);

  ___CBTPOP;
}

void arrayList_t::append (object_t* val) {
  ___CBTPUSH;

  ensureCapacity (1);
  _buf[_length] = val;
  ++_length;

  ___CBTPOP;
}

void arrayList_t::appendarAll (list_t* other) {
  ___CBTPUSH;

  listIterator_t* iter = other->iterator ();
  int len = other->length ();
  ensureCapacity (len);
  for (int i=0; i<len; ++i) {
    object_t* obj = iter->next ();
    _buf[_length] = obj;
    obj->reserve ();
    ++_length;
  }
  iter->dump ();

  ___CBTPOP;
}

void arrayList_t::clear (void) {
  ___CBTPUSH;

  for (int i=0; i<_length; ++i) {
    _buf[i]->dump ();
  }
  _length = 0;

  ___CBTPOP;
}

bool arrayList_t::contains (object_t* val) {
  ___CBTPUSH;

  bool result = indexOf (val) > -1;

  ___CBTPOP;
  return result;
}

void arrayList_t::ensureCapacity (int additionalLength) {
  ___CBTPUSH;

  long int delta = (long int) _length + additionalLength - _capacity;
  if (delta > 0) {
    delta = 1 + (delta / _capacityQuantum);
    delta *= _capacityQuantum;
    _capacity += delta;
    _buf = (object_t**) mem_reallocate (_buf, _capacity * sizeof (void*));
  }

  ___CBTPOP;
}

void arrayList_t::erase (int index) {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index)

  --_length;
  _buf[index]->dump ();
  memmove (_buf + index, _buf + index + 1, sizeof (void*) * (_length - index));

  ___CBTPOP;
}

object_t* arrayList_t::get (int index) {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index)

  object_t* result = _buf[index];

  ___CBTPOP;
  return result;
}

int arrayList_t::indexOf (object_t* val) {
  ___CBTPUSH;

  int i = 0;
  while (i < _length && !val->equals (_buf[i])) {
    ++i;
  }

  ___CBTPOP;
  return i < _length ? i : -1;
}

void arrayList_t::insert (int index, object_t* val) {
  ___CBTPUSH;
  DBG_INDEX_CHECK_FOR_INSERT (index)

  ensureCapacity (1);
  memmove (_buf + index + 1, _buf + index, sizeof (void*) * (_length - index));
  _buf[index] = val;
  ++_length;

  ___CBTPOP;
}

void arrayList_t::insertar (int index, object_t* val) {
  ___CBTPUSH;

  insert (index, val);
  val->reserve ();

  ___CBTPOP;
}

listIterator_t* arrayList_t::iterator (void) {
  ___CBTPUSH;

  listIterator_t* result = new arrayListIterator_t (this);

  ___CBTPOP;
  return result;
}

int arrayList_t::length (void) {
  return _length;
}

object_t* arrayList_t::remove (int index) {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index)

  --_length;
  object_t* result = _buf[index];
  memmove (_buf + index, _buf + index + 1, sizeof (void*) * (_length - index));

  ___CBTPOP;
  return result;
}

object_t* arrayList_t::removeLast (void) {
  ___CBTPUSH;
#ifdef DEBUG_MODE
  if (_length == 0) {
    errors_printMessageAndExit ("List is empty");
  }
#endif

  --_length;
  object_t* result = _buf[_length];

  ___CBTPOP;
  return result;
}

void arrayList_t::set (int index, object_t* val) {
  ___CBTPUSH;
  DBG_INDEX_CHECK (index)

  _buf[index]->dump ();
  _buf[index] = val;

  ___CBTPOP;
}

void arrayList_t::setar (int index, object_t* val) {
  ___CBTPUSH;

  set (index, val);
  val->reserve ();

  ___CBTPOP;
}

void arrayList_t::setCapacityQuantum (int val) {
  _capacityQuantum = val;
}

void arrayList_t::sort (int (* compare)(object_t*, object_t*)) {
  ___CBTPUSH;

  if (_length > 1) {
    for (int i = 1; i < _length; ++i) {
      object_t* obj = _buf[i];
      int j = 0;
      while (j < i && compare (obj, _buf[j]) > 0) {
        ++j;
      }
      for (int k = i; k > j; --k) {
        _buf[k] = _buf[k - 1];
      }
      _buf[j] = obj;
    }
  }

  ___CBTPOP;
}

void arrayList_t::trimToSize (void) {
  ___CBTPUSH;

  if (_capacity > _length) {
    _capacity = _length;
    _buf = (object_t**) mem_reallocate (_buf, _capacity * sizeof (void*));
  }

  ___CBTPOP;
}
