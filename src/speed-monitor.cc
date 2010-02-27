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

#include <iostream>

#include "speed-monitor.h"
#include "utils.h"

SpeedMonitor::SpeedMonitor () :
    config (Config::Instance ()),
    ulimit (Utils::parseInt (config.get_property (Config::MAX_UPLOAD_SPEED))),
    dlimit (Utils::parseInt (config.get_property (Config::MAX_DOWNLOAD_SPEED)))
{
    config.get_signal (Config::MAX_UPLOAD_SPEED).connect (sigc::mem_fun (*this, &SpeedMonitor::property_changed));
    config.get_signal (Config::MAX_DOWNLOAD_SPEED).connect (sigc::mem_fun (*this, &SpeedMonitor::property_changed));

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
        update (d);

        delete speeds[d];
        speeds.erase (d);
    }
}

int
SpeedMonitor::update_downloaded (int bytes)
{
    dtotal += bytes;

    int waitval = 0;

    if (dlimit != -1) {
        if (dtotal > dlimit) {
            waitval = time_to_next ();
        }
    }

    return waitval;
}

int
SpeedMonitor::update_uploaded (int bytes)
{
    utotal += bytes;

    int waitval = 0;

    if (ulimit != -1) {
        if (utotal > ulimit) {
            waitval = time_to_next ();
        }
    }

    return waitval;
}

int
SpeedMonitor::time_to_next ()
{
    struct timeval now;
    gettimeofday (&now, NULL);

    return 1000000 - (now.tv_sec - ltime.tv_sec) * 1000000 - now.tv_usec + ltime.tv_usec;
}

void*
SpeedMonitor::monitor_main (void *m)
{
    SpeedMonitor *monitor = (SpeedMonitor*) m;
    std::map<Download*,SpeedObject*>::iterator iter;

    monitor->running = true;

    while (monitor->running) {
        gettimeofday (&monitor->ltime, NULL);

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

void
SpeedMonitor::property_changed (const std::string &key, const std::string &val)
{
    if (key == Config::MAX_UPLOAD_SPEED) {
        ulimit = Utils::parseInt (val);
    } else if (key == Config::MAX_DOWNLOAD_SPEED) {
        dlimit = Utils::parseInt (val);
    }
}
