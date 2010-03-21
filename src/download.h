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
    STATE_CANNOT_CONNECT,
    STATE_PROTOCOL_ERROR,
} DownloadState;

/**
 * Base class for all downloads.
 * Class provides a common base class for all downloads and an interface for the
 * rest of the application to access and modify all the different types of
 * downloads in a common way.
 */

class Download {
    public:
        Download ();
        ~Download ();

        /**
         * Start asyncronous request for download info.
         * Method will start an asynchronous request to get the information
         * about the download for things such as availability and file size.
         */
        virtual void start_get_info ();

        /**
         * Start asyncronous download of the resource.
         * Method will start an asynchronous download of the resource.  All
         * state handling is done through signals so no return value is required
         * to check if the download started successfully.
         */
        virtual void start_download ();

        /**
         * Pause all tranfers going on between client and server.
         */
        virtual void pause ();

        /**
         * Set the internal state to queued.
         * Setting the state to queued is mainly useful for DownloadList.  It
         * allows that class to easly keep track of what is queued and what is
         * not so that when a download slot becomes available, it can start a
         * transfer
         */
        virtual void queue ();

        /**
         * Load download from data map and setup internal data.
         * Data is loaded in using KEY_* strings and the internal state and data
         * is loaded using the input map.
         * @param data map containing the data to load into a download object
         * @return true on successful load of data and false when the data load
         *         failed and the download is not valid
         */
        virtual bool startup (std::map<std::string,std::string> &data);

        /**
         * Close transfers and store internal data in map.
         * All data transfers to and from server are closed and the data is
         * saved.  The internal data from the download is then stored in the map
         * passed in.  The map will be saved and reloaded with startup when the
         * program is started again.
         * @param data map used to store data about internals of download
         * @return true if download was successfully shutdown and stored in the
         *         map.  If something went wrong the method will return false
         *         and the data should be discarded.
         */
        virtual bool shutdown (std::map<std::string,std::string> &data);

        /**
         * Close transfers and delete all data saved from the drive.
         * All data transfers to and from the server are closed and all data
         * that has been saved to the disk is deleted.  This does not clear any
         * internal data structures.
         */
        virtual void delete_data ();

        sigc::signal<void, Download*, DownloadState> &signal_state_changed () { return state_changed; }

        std::string get_display_name () const { return display_name; }
        void set_display_name (std::string &new_name) { display_name = new_name; set_state (state); }

        DownloadState get_state () const { return state; }
        std::string get_status () const { return status; }

        int get_dsize () const { return dsize; }
        int get_dtrans () const { return dtrans; }
        int get_usize () const { return usize; }
        int get_utrans () const { return utrans; }
        int get_uspeed () const;
        int get_dspeed () const;

        static const std::string KEY_DISPLAY_NAME;
        static const std::string KEY_URL;
        static const std::string KEY_DOWNLOADED;
        static const std::string KEY_SIZE;
        static const std::string KEY_STATE;
        static const std::string KEY_LOCATION;
        static const std::string KEY_MATCH;

    protected:
        DownloadState state;
        std::string status;

        std::string display_name;

        std::string filename;

        SpeedObject *so;
        int dsize, dtrans, usize, utrans;

        /**
         * Set internal download state.
         * Method will set the internal state flag of the download to the
         * specified state and send out a signal informing listeners that the
         * state has changed.
         * @param state new state to change to
         */
        void set_state (DownloadState state);

    private:
        sigc::signal<void, Download*, DownloadState> state_changed;
};

#endif /* __DOWNLOAD_H__ */
