/*
 *      speed-object.cc
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

#include "speed-object.h"

SpeedObject::SpeedObject () :
    monitor (SpeedMonitor::Instance ())
{

}

SpeedObject::~SpeedObject ()
{

}

int
SpeedObject::update_downloaded (int bytes)
{
    dtotal += bytes;

    int val = monitor.update_downloaded (bytes);
    if (val > 0) {
        return val;
    }

    return 0;
}

int
SpeedObject::update_uploaded (int bytes)
{
    utotal += bytes;

    int val = monitor.update_uploaded (bytes);
    if (val > 0) {
        return val;
    }

    return 0;
}

void
SpeedObject::update ()
{
    putotal = utotal;
    pdtotal = dtotal;

    utotal = dtotal = 0;
}

void
SpeedObject::wait (int time)
{
    if (time > 0) {
        usleep (time);
    }
}
