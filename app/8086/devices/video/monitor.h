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

#ifndef ___DEBUG_DEVICES_VIDEO__MONITOR_INCLUDED
#define ___DEBUG_DEVICES_VIDEO__MONITOR_INCLUDED

#include <gtk/gtk.h>
#include <pthread.h>
#include <lombok/base/class.h>

class monitor_t : public object_t {
private:
  GtkWidget* m_window;
  GtkWidget* m_drawingArea;
  cairo_surface_t* m_sourceSurface;
  pthread_mutex_t* m_mutex;
  bool m_gtkThreadDone;

public:
  monitor_t (cairo_surface_t* sourceSurface);
  virtual ~monitor_t (void);

  virtual bool refresh (void);

private:
  void killGtkThread (void);

  static gboolean drawCallback (GtkWidget* widget, cairo_t* cr, gpointer monitor);
  static void* gtkThreadMain (void* arg);
  static void gtkThreadQuit (GtkWidget* widget, gpointer monitor);
  static gboolean gtkWidgetQueueDraw (gpointer monitor);

  REFL_DECL
};

#endif
