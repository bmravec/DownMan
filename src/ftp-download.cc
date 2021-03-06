/*
 *      ftp-download.cc
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

#include "ftp-download.h"
#include "ftp-connection.h"
#include "utils.h"
#include "socket.h"

const DRegex FtpDownload::MATCH_REGEX ("ftp://([^/:]+?)(:[0-9]+)?(/[^[:space:]]+/([^[:space:]\\?]+))(\\?[^[:space:]]+)?");

const std::string FtpDownload::KEY_LOCAL_PATH ("local-path");
const std::string FtpDownload::KEY_REMOTE_PATH ("remote-path");
const std::string FtpDownload::KEY_REMOTE_HOST ("remote-host");
const std::string FtpDownload::KEY_REMOTE_PORT ("remote-port");

FtpDownload::FtpDownload () : running (false), remote_port (0)
{

}

FtpDownload::FtpDownload (std::vector<std::string> &m) :
    running (false), remote_port (0)
{
    local_path = Utils::createDownloadFilename (m[4]);

    display_name = m[4];

    remote_host = m[1];
    remote_path = m[3];

    if (m[2].size () > 0) {
        remote_port = Utils::parseInt (m[2].substr (1));
    }
}

FtpDownload::~FtpDownload ()
{
    if (running) {
        running = false;
        pthread_join (thread, NULL);
    }
}

void
FtpDownload::start_get_info ()
{
    pthread_create (&thread, NULL, FtpDownload::static_run_info, this);
}

void
FtpDownload::start_download ()
{
    pthread_create (&thread, NULL, FtpDownload::static_run_download, this);
}

void
FtpDownload::pause ()
{
    if (running) {
        running = false;
        pthread_join (thread, NULL);
    }

    set_state (STATE_PAUSED);
}

bool
FtpDownload::startup (std::map<std::string,std::string> &data)
{
    if (MATCH_REGEX == data[KEY_MATCH] && data.count (KEY_DISPLAY_NAME) > 0 &&
        data.count (KEY_DOWNLOADED) > 0 && data.count (KEY_SIZE) > 0 &&
        data.count (KEY_STATE) > 0 && data.count (KEY_LOCAL_PATH) > 0 &&
        data.count (KEY_REMOTE_HOST) > 0 && data.count (KEY_REMOTE_PORT) > 0 &&
        data.count (KEY_REMOTE_PATH) > 0) {

        display_name = data[KEY_DISPLAY_NAME];
        dtrans = Utils::parseInt (data[KEY_DOWNLOADED]);
        dsize = Utils::parseInt (data[KEY_SIZE]);
        state = (DownloadState) Utils::parseInt (data[KEY_STATE]);

        local_path= data[KEY_LOCAL_PATH];
        remote_path = data[KEY_REMOTE_PATH];
        remote_host = data[KEY_REMOTE_HOST];
        remote_port = Utils::parseInt (data[KEY_REMOTE_PORT]);

        return true;
    } else {
        return false;
    }
}

bool
FtpDownload::shutdown (std::map<std::string, std::string> &data)
{
    if (running) {
        running = false;
        pthread_join (thread, NULL);
    }

    if (state == STATE_DOWNLOADING) {
        state = STATE_QUEUED;
    }

    data[KEY_DISPLAY_NAME] = display_name;
    data[KEY_DOWNLOADED] = Utils::formatInt (dtrans);
    data[KEY_SIZE] = Utils::formatInt (dsize);
    data[KEY_STATE] = Utils::formatInt ((int) state);
    data[KEY_MATCH] = MATCH_REGEX.get_string ();
    data[KEY_LOCAL_PATH] = local_path;
    data[KEY_REMOTE_PATH] = remote_path;
    data[KEY_REMOTE_HOST] = remote_host;
    data[KEY_REMOTE_PORT] = Utils::formatInt (remote_port);

    return true;
}

void
FtpDownload::run_info ()
{
    running = true;
    set_state (STATE_INFO);

    FtpConnection conn (remote_host, remote_port);
    if (conn.get_file_size (remote_path, &dsize)) {
        status = "Online";
        set_state (STATE_INFO_COMPLETED);
    } else {
        status = "Offline";
        set_state (STATE_NOT_FOUND);
    }

    running = false;
}

void
FtpDownload::run_download ()
{
    running = true;
    status = "Downloading...";
    set_state (STATE_DOWNLOADING);

    so = SpeedMonitor::Instance ().get (this);

    bool append = false;
    bool connected = false;

    FtpConnection conn (remote_host, remote_port);
    if (Utils::getFileSize (local_path) == dtrans) {
        append = true;
    }

    if (append) {
        if (conn.send_get_request (remote_path, dtrans)) {
            connected = true;
        } else if (conn.send_get_request (remote_path)) {
            append = false;
            connected = true;
        } else {
            status = "Not Found";
            set_state (STATE_NOT_FOUND);
        }
    } else {
        if (conn.send_get_request (remote_path)) {
            connected = true;
        } else {
            status = "Not Found";
            set_state (STATE_NOT_FOUND);
        }
    }

    if (connected) {
        if (append) {
            ofile.open (local_path.c_str (), std::ios::out | std::ios::binary | std::ios::app);
        } else {
            ofile.open (local_path.c_str (), std::ios::out | std::ios::binary);
            dtrans = 0;
        }

        int len;
        char *buff = new char[2048];

        while (running) {
            len = conn.read (buff, 2048);
            if (len <= 0) {
                break;
            }

            ofile.write (buff, len);
            dtrans += len;
            so->wait (so->update_downloaded (len));
        }

        delete buff;
        ofile.close ();

        if (running) {
            set_state (STATE_COMPLETED);
        }

        status = "";
    }

    so = NULL;
    SpeedMonitor::Instance ().remove (this);

    running = false;
}

void*
FtpDownload::static_run_info (void *download)
{
    ((FtpDownload*) download)->run_info ();

    return NULL;
}

void*
FtpDownload::static_run_download (void *download)
{
    ((FtpDownload*) download)->run_download ();

    return NULL;
}
