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
#include "utils.h"

HttpDownload::HttpDownload (std::string &url) : Download (url)
{
    this->url = url;

    size_t pos = url.rfind ("/");
    name = url.substr (pos + 1);
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
    HttpDownload *d = (HttpDownload*) download;

    d->running = true;
    d->status = "Downloading...";
    d->set_state (STATE_DOWNLOADING);

    d->curl = curl_easy_init ();
    curl_easy_setopt (d->curl, CURLOPT_URL, d->url.c_str ());

    curl_easy_setopt (d->curl, CURLOPT_VERBOSE, 1);

    curl_easy_setopt (d->curl, CURLOPT_WRITEFUNCTION, &HttpDownload::write_function);
    curl_easy_setopt (d->curl, CURLOPT_WRITEDATA, d);

    curl_easy_setopt (d->curl, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt (d->curl, CURLOPT_PROGRESSFUNCTION, &HttpDownload::progress_function);
    curl_easy_setopt (d->curl, CURLOPT_PROGRESSDATA, d);

    d->ofile.open (d->name.c_str (), std::ios::out | std::ios::binary);

    curl_easy_perform (d->curl);

    curl_easy_cleanup (d->curl);
    d->curl = NULL;

    d->ofile.close ();

    d->status = "Completed";
    d->set_state (STATE_COMPLETED);

    d->running = false;
}

size_t
HttpDownload::write_function (void *ptr,
                              size_t size,
                              size_t nmemb,
                              void *stream)
{
    HttpDownload *d = (HttpDownload*) stream;

    d->ofile.write ((char*) ptr, size * nmemb);

    return size * nmemb;
}

int
HttpDownload::progress_function (HttpDownload *d,
                                 double dt, double dn,
                                 double ut, double un)
{
    std::string dnow = size_to_string (dn);
    std::string dtot = size_to_string (dt);

    std::cout << "D (" << dnow << "," << dtot << ") ";
    std::cout << std::endl;

    d->dsize = dt;
    d->dtrans = dn;

    return 0;
}
