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
#include "videoCardConstants.h"
#include "monitor.h"

REFL_IMPL (monitor_t, object_t)

monitor_t::monitor_t (cairo_surface_t* sourceSurface) : m_sourceSurface (sourceSurface), m_gtkThreadDone (false) {
  ___CBTPUSH;

  int gtkInitArgc = 0;
  gtk_init (&gtkInitArgc, NULL); /* TODO: Should this occur only once???!!! */

  m_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (m_window), "Monitor");
  g_signal_connect (m_window, "destroy", G_CALLBACK (gtkThreadQuit), this);

  m_drawingArea = gtk_drawing_area_new ();
  gtk_widget_set_size_request (m_drawingArea, SCREEN_WIDTH + 2 * X_MARGIN, SCREEN_HEIGHT + 2 * Y_MARGIN);
  g_signal_connect (m_drawingArea, "draw", G_CALLBACK (drawCallback), this);

  gtk_container_add (GTK_CONTAINER (m_window), m_drawingArea);

  gtk_widget_show_all (m_window);

  m_mutex = (pthread_mutex_t*) mem_allocate ("pthread_mutex_t", sizeof (pthread_mutex_t), PTYPE_INTERNAL);
  pthread_mutex_init (m_mutex, NULL);

  pthread_t gtkThread;
  pthread_create (&gtkThread, NULL, gtkThreadMain, NULL);

  ___CBTPOP;
}

monitor_t::~monitor_t (void) {
  ___CBTPUSH;

  killGtkThread ();
  pthread_mutex_destroy (m_mutex);
  mem_release (m_mutex);

  ___CBTPOP;
}

gboolean monitor_t::drawCallback (GtkWidget* widget, cairo_t* dst, gpointer monitor) {
  ___CBTPUSH;

  cairo_surface_t* sourceSurface = ((monitor_t*) monitor)->m_sourceSurface;
  cairo_set_source_surface (dst, sourceSurface, 0, 0);
  cairo_paint (dst);

  ___CBTPOP;
  return true;
}

void monitor_t::killGtkThread (void) {
  ___CBTPUSH;

  pthread_mutex_lock (m_mutex);
  bool gtkMainQuitShouldBeCalled = !m_gtkThreadDone;
  m_gtkThreadDone = true;
  pthread_mutex_unlock (m_mutex);
  if (gtkMainQuitShouldBeCalled) {
    gtk_widget_destroy (m_drawingArea);
    gtk_widget_destroy (m_window);
    gtk_main_quit ();
  }

  ___CBTPOP;
}

void* monitor_t::gtkThreadMain (void* arg) {
  ___CBTPUSH;

  gtk_main ();

  ___CBTPOP;
  return NULL;
}

void monitor_t::gtkThreadQuit (GtkWidget* widget, gpointer monitor) {
  ___CBTPUSH;

  ((monitor_t*) monitor)->killGtkThread ();

  ___CBTPOP;
}

gboolean monitor_t::gtkWidgetQueueDraw (gpointer monitor) {
  ___CBTPUSH;

  gtk_widget_queue_draw (((monitor_t*) monitor)->m_drawingArea);

  ___CBTPOP;
  return G_SOURCE_REMOVE;
}

bool monitor_t::refresh (void) {
  ___CBTPUSH;

  pthread_mutex_lock (m_mutex);
  bool success = !m_gtkThreadDone;
  if (success) {
    g_idle_add (gtkWidgetQueueDraw, this);
  }
  pthread_mutex_unlock (m_mutex);

  ___CBTPOP;
  return success;
}
