/*
 *      download.h
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

class Download;

#ifndef __DOWNLOAD_H__
#define __DOWNLOAD_H__

#include <string>
#include <map>
#include <sigc++/sigc++.h>

typedef enum {
    STATE_NULL = 0,
    STATE_PAUSED,
    STATE_QUEUED,
    STATE_CONNECTING,
    STATE_DOWNLOADING,
    STATE_SEEDING,
    STATE_WAITING,
    STATE_HOLDING,
    STATE_DISABLED,
    STATE_COMPLETED,
    STATE_INFO,
    STATE_INFO_COMPLETED,
    STATE_NOT_FOUND,
} DownloadState;

class Download {
    public:
        Download ();
        ~Download ();

        void start_get_info ();
        void start_download ();

        void startup (const std::map<std::string,std::string> &data);
        std::map<std::string,std::string> *shutdown ();

        sigc::signal<void, DownloadState> &signal_state_changed () { return state_changed; }

    protected:
        DownloadState state;
        char *status;

        void set_state (DownloadState state);

    private:
        sigc::signal<void, DownloadState> state_changed;
};

#endif /* __DOWNLOAD_H__ */
