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

#include <curl/curl.h>

#include "http-download.h"

HttpDownload::HttpDownload (std::string &url) : Download (url)
{
    this->url = url;

    size_t pos = url.rfind ("/");
    name = url.substr (pos + 1);
}

HttpDownload::~HttpDownload ()
{
    if (running) {
        pthread_join (thread, NULL);
    }
}

void
HttpDownload::start_get_info ()
{
    pthread_create (&thread, NULL, HttpDownload::run_info, this);
}

void
HttpDownload::start_download ()
{
    pthread_create (&thread, NULL, HttpDownload::run_download, this);
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

    return NULL;
}

void*
HttpDownload::run_info (void *download)
{
    HttpDownload *d = (HttpDownload*) download;

    d->running = true;
    d->set_state (STATE_INFO);

    CURL *curl = curl_easy_init ();
    curl_easy_setopt (curl, CURLOPT_URL, d->url.c_str ());
    curl_easy_setopt (curl, CURLOPT_NOBODY, 1);
    curl_easy_perform (curl);

    double cl;
    curl_easy_getinfo (curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &cl);

    d->dsize = cl;

    int response;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &response);

    if (response >= 200 && response < 400) {
        d->status = "Online";
        d->set_state (STATE_INFO_COMPLETED);
    } else {
        d->status = "Offline";
        d->set_state (STATE_NOT_FOUND);
    }

    d->running = false;
}

void*
HttpDownload::run_download (void *download)
{

}
