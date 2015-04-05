/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2014, 2015 Gerard Visser.

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

#include <time.h>
#include <gtk/gtk.h>
#include <lombok/base/errors.h>
#include <lombok/base/mem.h>
#include "video/videoCardConstants.h"
#include "videoCard.h"

REFL_IMPL (videoCard_t, device_t)

#define ATTRIBUTE_INDEX_WDATA 0x3C0
#define ATTRIBUTE_RDATA       0x3C1
#define VIDEO_ENABLE          0x3C3
#define SEQUENCER_INDEX       0x3C4
#define SEQUENCER_DATA        0x3C5
#define DAC_RINDEX            0x3C7
#define DAC_WINDEX            0x3C8
#define DAC_DATA              0x3C9
#define GRAPHICS_INDEX        0x3CE
#define GRAPHICS_DATA         0x3CF
#define CRT_CONTROLLER_INDEX  0x3D4
#define CRT_CONTROLLER_DATA   0x3D5
#define STATUS                0x3DA

#define REFRESH_THREAD_SUSPENSION_REQUEST 1
#define REFRESH_THREAD_SUSPENDED          2
#define REFRESH_THREAD_TO_BE_TERMINATED   3

const int videoports[] = {0x3C0, 0x3C1, 0x3C2, 0x3C3, 0x3C4, 0x3C5, 0x3C6, 0x3C7,
                          0x3C8, 0x3C9, 0x3CA, 0x3CB, 0x3CC, 0x3CD, 0x3CE, 0x3CF,
                          0x3D0, 0x3D1, 0x3D2, 0x3D3, 0x3D4, 0x3D5, 0x3D6, 0x3D7,
                          0x3D8, 0x3D9, 0x3DA, 0x3DB, 0x3DC, 0x3DD, 0x3DE, 0x3DF, -1};

videoCard_t::videoCard_t (memory_t* memory) : m_refreshThreadState (0) {
  ___CBTPUSH;

  m_drawingSurface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, SCREEN_WIDTH + 10, SCREEN_HEIGHT + 10);
  m_video = new videoCardCore_t (memory, m_drawingSurface);
  m_monitor = new monitor_t (m_drawingSurface);
  m_mutex = (pthread_mutex_t*) mem_allocate ("pthread_mutex_t", sizeof (pthread_mutex_t), PTYPE_INTERNAL);
  m_condVar = (pthread_cond_t*) mem_allocate ("pthread_cond_t", sizeof (pthread_cond_t), PTYPE_INTERNAL);
  pthread_cond_init (m_condVar, NULL);
  pthread_mutex_init (m_mutex, NULL);

  pthread_attr_t threadAttributes;
  pthread_attr_init (&threadAttributes);
  pthread_attr_setdetachstate (&threadAttributes, PTHREAD_CREATE_JOINABLE);
  pthread_create (&m_refreshThread, &threadAttributes, refreshThreadMain, this);
  pthread_attr_destroy (&threadAttributes);

  ___CBTPOP;
}

videoCard_t::~videoCard_t (void) {
  ___CBTPUSH;

  terminateRefreshThread ();
  m_video->dump ();
  m_monitor->dump ();
  cairo_surface_destroy (m_drawingSurface);
  pthread_cond_destroy (m_condVar);
  pthread_mutex_destroy (m_mutex);
  mem_release (m_condVar);
  mem_release (m_mutex);

  ___CBTPOP;
}

const int* videoCard_t::ports (void) {
  return videoports;
}

int videoCard_t::readByte (unsigned short port) {
  ___CBTPUSH;

  int result = -1;
  switch (port) {
  case ATTRIBUTE_INDEX_WDATA:
    result = m_video->getAttributeIndex ();
    break;

  case ATTRIBUTE_RDATA:
    result = m_video->getAttributeData ();
    break;

  case VIDEO_ENABLE:
    result = m_video->getVideoEnabled ();
    break;

  case SEQUENCER_INDEX:
    result = m_video->getSequencerIndex ();
    break;

  case SEQUENCER_DATA:
    result = m_video->getSequencerData ();
    break;

  case DAC_RINDEX:
    result = m_video->getDacReadIndex ();
    break;

  case DAC_WINDEX:
    result = m_video->getDacWriteIndex ();
    break;

  case DAC_DATA:
    result = m_video->getDacData ();
    break;

  case GRAPHICS_INDEX:
    result = m_video->getGraphicsIndex ();
    break;

  case GRAPHICS_DATA:
    result = m_video->getGraphicsData ();
    break;

  case CRT_CONTROLLER_INDEX:
    result = m_video->getCrtIndex ();
    break;

  case CRT_CONTROLLER_DATA:
    result = m_video->getCrtData ();
    break;

  case STATUS:
    result = 0;
    m_attrWriteData = false;
  }

  ___CBTPOP;
  return result;
}

int videoCard_t::readWord (unsigned short port) {
  ___CBTPUSH;

  int result = -1;

  ___CBTPOP;
  return result;
}

void* videoCard_t::refreshThreadMain (void* _videoCard) {
  ___CBTPUSH;

  videoCard_t* videoCard = (videoCard_t*) _videoCard;
  struct timespec refreshTime, refreshTimeRemaining;
  refreshTime.tv_nsec = 250000000;
  refreshTime.tv_sec = 0;

  bool done;
  do {
    nanosleep (&refreshTime, &refreshTimeRemaining);
    videoCard->m_video->drawScreen ();
    done = !videoCard->m_monitor->refresh ();
  } while (!(done || videoCard->terminationRequested ()));

  ___CBTPOP;
}

void videoCard_t::suspendRefreshThread (void) {
  ___CBTPUSH;

  pthread_mutex_lock (m_mutex);
  if (m_refreshThreadState == 0) {
    m_refreshThreadState = REFRESH_THREAD_SUSPENSION_REQUEST;
  }
  bool suspensionHasBeenRequested = m_refreshThreadState == REFRESH_THREAD_SUSPENSION_REQUEST;
  pthread_mutex_unlock (m_mutex);

  if (suspensionHasBeenRequested) {
    struct timespec waitingTime, remainingTime;
    waitingTime.tv_nsec = 125000000;
    waitingTime.tv_sec = 0;

    pthread_mutex_lock (m_mutex);
    bool threadWaiting = m_refreshThreadState >= REFRESH_THREAD_SUSPENDED;
    pthread_mutex_unlock (m_mutex);
    while (!threadWaiting) {
      nanosleep (&waitingTime, &remainingTime);
      pthread_mutex_lock (m_mutex);
      threadWaiting = m_refreshThreadState >= REFRESH_THREAD_SUSPENDED;
      pthread_mutex_unlock (m_mutex);
    }
  }

  ___CBTPOP;
}

void videoCard_t::terminateRefreshThread (void) {
  ___CBTPUSH;

  pthread_mutex_lock (m_mutex);
  m_refreshThreadState = REFRESH_THREAD_TO_BE_TERMINATED;
  pthread_cond_signal (m_condVar);
  pthread_mutex_unlock (m_mutex);
  pthread_join (m_refreshThread, NULL);

  ___CBTPOP;
}

bool videoCard_t::terminationRequested (void) {
  ___CBTPUSH;

  pthread_mutex_lock (m_mutex);
  if (m_refreshThreadState == REFRESH_THREAD_SUSPENSION_REQUEST) {
    m_refreshThreadState = REFRESH_THREAD_SUSPENDED;
    pthread_cond_wait (m_condVar, m_mutex);
    if (m_refreshThreadState == REFRESH_THREAD_SUSPENDED) {
      m_refreshThreadState = 0;
    }
  }
  bool result = m_refreshThreadState == REFRESH_THREAD_TO_BE_TERMINATED;
  pthread_mutex_unlock (m_mutex);

  ___CBTPOP;
  return result;
}

void videoCard_t::writeByte (unsigned short port, int val) {
  ___CBTPUSH;

  switch (port) {
  case ATTRIBUTE_INDEX_WDATA:
    if (m_attrWriteData) {
      m_video->setAttributeData (val);
    } else {
      m_video->setAttributeIndex (val);
    }
    m_attrWriteData = !m_attrWriteData;
    break;

  case VIDEO_ENABLE: {
    bool enableVideo = val & 1;
    bool videoEnabled = m_video->getVideoEnabled ();
    if (enableVideo) {
      if (!videoEnabled) {
        m_video->setVideoEnabled (true);
        pthread_mutex_lock (m_mutex);
        pthread_cond_signal (m_condVar);
        pthread_mutex_unlock (m_mutex);
      }
    } else if (videoEnabled) {
      m_video->setVideoEnabled (false);
      suspendRefreshThread ();
      m_video->clearScreen ();
      m_monitor->refresh ();
    }
  } break;

  case SEQUENCER_INDEX:
    m_video->setSequencerIndex (val);
    break;

  case SEQUENCER_DATA:
    m_video->setSequencerData (val);
    break;

  case DAC_RINDEX:
    m_video->setDacReadIndex (val);
    break;

  case DAC_WINDEX:
    m_video->setDacWriteIndex (val);
    break;

  case DAC_DATA:
    m_video->setDacData (val);
    break;

  case GRAPHICS_INDEX:
    m_video->setGraphicsIndex (val);
    break;

  case GRAPHICS_DATA:
    m_video->setGraphicsData (val);
    break;

  case CRT_CONTROLLER_INDEX:
    m_video->setCrtIndex (val);
    break;

  case CRT_CONTROLLER_DATA:
    m_video->setCrtData (val);
  }

  ___CBTPOP;
}

void videoCard_t::writeWord (unsigned short port, int val) {
  ___CBTPUSH;
  ___CBTPOP;
}
