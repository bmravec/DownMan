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

#include "speed-object.h"
#include "url.h"

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
        Download (Url &url, std::string match_str = "");
        Download (std::string &url, std::string match_str = "");
        ~Download ();

        virtual void start_get_info ();
        virtual void start_download ();

        virtual bool startup (std::map<std::string,std::string> &data);
        virtual std::map<std::string,std::string> *shutdown ();

        sigc::signal<void, Download*, DownloadState> &signal_state_changed () { return state_changed; }

        const std::string get_status () { return status; }
        const std::string get_name () { return name; }
        DownloadState get_state () { return state; }

        int get_dsize () { return dsize; }
        int get_dtrans () { return dtrans; }
        int get_usize () { return usize; }
        int get_utrans () { return utrans; }
        int get_uspeed ();
        int get_dspeed ();

        std::string get_match_string () { return match_str; }

        static std::string KEY_NAME;
        static std::string KEY_URL;
        static std::string KEY_DOWNLOADED;
        static std::string KEY_SIZE;
        static std::string KEY_STATE;
        static std::string KEY_LOCATION;
        static std::string KEY_MATCH;

    protected:
        DownloadState state;
        Url url;
        std::string name;
        std::string status;
        std::string filename;
        std::string match_str;
        int dsize, dtrans, usize, utrans;
        SpeedObject *so;

        void set_state (DownloadState state);

    private:
        sigc::signal<void, Download*, DownloadState> state_changed;
};

#endif /* __DOWNLOAD_H__ */
