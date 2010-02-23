/*
 *      speed-monitor.h
 *
 *      Copyright 2010 Brett Mravec <brett.mravec@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

class SpeedMonitor;

#ifndef __SPEED_MONITOR_H__
#define __SPEED_MONITOR_H__

#include <map>
#include <sigc++/sigc++.h>
#include <sys/time.h>

#include <pthread.h>

#include "speed-object.h"
#include "download.h"
#include "app-config.h"

class SpeedMonitor {
    public:
        static SpeedMonitor &Instance () {
            static SpeedMonitor monitor;
            return monitor;
        }

        SpeedObject *get (Download *d);
        void remove (Download *d);

        sigc::signal<void, Download*> &signal_update () { return update; }

        int update_downloaded (int bytes);
        int update_uploaded (int bytes);

    private:
        SpeedMonitor ();
        ~SpeedMonitor ();

        int time_to_next ();

        Config &config;

        struct timeval ltime;
        int putotal, pdtotal, utotal, dtotal, ulimit, dlimit;
        std::map<Download*,SpeedObject*> speeds;

        pthread_t thread;
        bool running;
        static void *monitor_main (void *m);

        sigc::signal<void, Download*> update;
};

#endif /* __SPEED_MONITOR_H__ */
