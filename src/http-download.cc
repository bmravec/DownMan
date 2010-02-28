/*
 *      http-download.cc
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

#include "http-download.h"
#include "http-connection.h"
#include "utils.h"
#include "socket.h"

void *static_main_info (void*);
void *static_main_download (void*);

HttpDownload::HttpDownload (Url &url) : Download (url, "http")
{
    filename = Utils::createDownloadFilename (url.get_name ());
}

HttpDownload::HttpDownload (std::string &url) : Download (url)
{

}

HttpDownload::~HttpDownload ()
{
    if (running) {
        running = false;
        pthread_join (thread, NULL);
    }
}

void
HttpDownload::start_get_info ()
{
    pthread_create (&thread, NULL, HttpDownload::static_run_info, this);
}

void
HttpDownload::start_download ()
{
    pthread_create (&thread, NULL, HttpDownload::static_run_download, this);
}

void
HttpDownload::startup (const std::map<std::string,std::string> &data)
{
    std::cout << "HttpDownload::startup (data): stub\n";
}

std::map<std::string,std::string>*
HttpDownload::shutdown ()
{
    std::cout << "HttpDownload::shutdown (): stub\n";
    std::string key;

    std::map<std::string, std::string> *data = new std::map<std::string, std::string>;

    if (running) {
        running = false;
        pthread_join (thread, NULL);
    }

    if (state == STATE_DOWNLOADING) {
        state = STATE_QUEUED;
    }

    key = "name";
    (*data)[key] = url.get_name ();

    key = "url";
    (*data)[key] = url.get_url ();

    key = "downloaded";
    (*data)[key] = Utils::formatInt (dtrans);

    key = "total";
    (*data)[key] = Utils::formatInt (dsize);

    key = "state";
    (*data)[key] = Utils::formatInt ((int) state);

    key = "location";
    (*data)[key] = filename;

    key = "match";
    (*data)[key] = match_str;

    return data;
}

void
HttpDownload::run_info ()
{
    int len;

    running = true;
    set_state (STATE_INFO);

    HttpConnection conn;
    conn.send_head_request (url);

    dsize = conn.get_content_length ();

    int rc = conn.get_response_code ();

    if (rc >= 200 && rc < 400) {
        status = "Online";
        set_state (STATE_INFO_COMPLETED);
    } else {
        status = "Offline";
        set_state (STATE_NOT_FOUND);
    }

    running = false;
}

void
HttpDownload::run_download ()
{
    running = true;
    status = "Downloading...";
    set_state (STATE_DOWNLOADING);

    so = SpeedMonitor::Instance ().get (this);

    HttpConnection conn;
    conn.send_get_request (url);

    int rc = conn.get_response_code ();
    if (rc >= 400 || rc < 200) {
        status = "Not Found";
        set_state (STATE_NOT_FOUND);
    } else {
        ofile.open (filename.c_str (), std::ios::out | std::ios::binary);

        dtrans = 0;
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

        ofile.close ();

        if (running) {
            status = "Completed";
            set_state (STATE_COMPLETED);
        }
    }

    so = NULL;
    SpeedMonitor::Instance ().remove (this);

    running = false;
}

void*
HttpDownload::static_run_info (void *download)
{
    ((HttpDownload*) download)->run_info ();

    return NULL;
}

void*
HttpDownload::static_run_download (void *download)
{
    ((HttpDownload*) download)->run_download ();

    return NULL;
}
