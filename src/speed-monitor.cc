/*
 *      speed-monitor.cc
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

#include "speed-monitor.h"

SpeedMonitor::SpeedMonitor () :
    config (Config::Instance ())
{
    pthread_create (&thread, NULL, &SpeedMonitor::monitor_main, this);
}

SpeedMonitor::~SpeedMonitor ()
{
    if (running) {
        running = false;
        pthread_cancel (thread);
    }
}

SpeedObject*
SpeedMonitor::get (Download *d)
{
    SpeedObject *obj = NULL;

    if (speeds.count (d) > 0) {
        obj = speeds[d];
    } else {
        obj = new SpeedObject ();
        speeds[d] = obj;
    }

    return obj;
}

void
SpeedMonitor::remove (Download *d)
{
    if (speeds.count (d) > 0) {
        delete speeds[d];
        speeds.erase (d);
    }
}

int
SpeedMonitor::update_downloaded (int bytes)
{
    dtotal += bytes;
    //TODO: Return waits

    return 0;
}

int
SpeedMonitor::update_uploaded (int bytes)
{
    utotal += bytes;
    //TODO: Return waits

    return 0;
}

void*
SpeedMonitor::monitor_main (void *m)
{
    SpeedMonitor *monitor = (SpeedMonitor*) m;
    std::map<Download*,SpeedObject*>::iterator iter;

    monitor->running = true;
    monitor->ltime = clock ();

    while (monitor->running) {
        monitor->putotal = monitor->utotal;
        monitor->pdtotal = monitor->dtotal;
        monitor->utotal = monitor->dtotal = 0;

        for (iter = monitor->speeds.begin (); iter != monitor->speeds.end (); iter++) {
            monitor->update (iter->first);
            iter->second->update ();
        }

        usleep (1000000);
    }
}
