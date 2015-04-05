/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2013 Gerard Visser.

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

#include <lombok/testing/testUtils.h>
#include "classTests.h"

/* f
   e:b
   d:c:b:a */
static void testIsAssignableFrom (void) {
  ___BTPUSH;
  printTest ("class_t::isAssignableFrom");

  class_t a ("a", 0, NULL);
  class_t b ("b", 0, &a);
  class_t c ("c", 0, &b);
  class_t d ("d", 0, &c);
  class_t e ("e", 0, &b);
  class_t f ("f", 0, NULL);

  assert (a.isAssignableFrom (&a));
  assert (a.isAssignableFrom (&b));
  assert (a.isAssignableFrom (&c));
  assert (a.isAssignableFrom (&d));
  assert (a.isAssignableFrom (&e));
  assert (!a.isAssignableFrom (&f));

  assert (!b.isAssignableFrom (&a));
  assert (b.isAssignableFrom (&b));
  assert (b.isAssignableFrom (&c));
  assert (b.isAssignableFrom (&d));
  assert (b.isAssignableFrom (&e));
  assert (!b.isAssignableFrom (&f));

  assert (!c.isAssignableFrom (&a));
  assert (!c.isAssignableFrom (&b));
  assert (c.isAssignableFrom (&c));
  assert (c.isAssignableFrom (&d));
  assert (!c.isAssignableFrom (&e));
  assert (!c.isAssignableFrom (&f));

  assert (!d.isAssignableFrom (&a));
  assert (!d.isAssignableFrom (&b));
  assert (!d.isAssignableFrom (&c));
  assert (d.isAssignableFrom (&d));
  assert (!d.isAssignableFrom (&e));
  assert (!d.isAssignableFrom (&f));

  assert (!e.isAssignableFrom (&a));
  assert (!e.isAssignableFrom (&b));
  assert (!e.isAssignableFrom (&c));
  assert (!e.isAssignableFrom (&d));
  assert (e.isAssignableFrom (&e));
  assert (!e.isAssignableFrom (&f));

  assert (!f.isAssignableFrom (&a));
  assert (!f.isAssignableFrom (&b));
  assert (!f.isAssignableFrom (&c));
  assert (!f.isAssignableFrom (&d));
  assert (!f.isAssignableFrom (&e));
  assert (f.isAssignableFrom (&f));

  performMemoryTest ();

  printOk ();
  ___BTPOP;
}

const struct classTestsStruct classTests = {
  testIsAssignableFrom
};
