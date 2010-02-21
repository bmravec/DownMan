/*
 *      speed-object.h
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

class SpeedObject;

#ifndef __SPEED_OBJECT_H__
#define __SPEED_OBJECT_H__

#include "speed-monitor.h"

class SpeedObject {
    public:
        SpeedObject ();
        ~SpeedObject ();

        int update_downloaded (int bytes);
        int update_uploaded (int bytes);
        void update ();

        void wait (int time);

        int get_uspeed () { return putotal; }
        int get_dspeed () { return pdtotal; }

    private:
        int ulimit, dlimit;
        int utotal, dtotal;
        int putotal, pdtotal;

        SpeedMonitor &monitor;
};

#endif /* __SPEED_OBJECT_H__ */
