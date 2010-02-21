/*
 *      http-download.h
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

class HttpDownload;

#ifndef __HTTP_DOWNLOAD_H__
#define __HTTP_DOWNLOAD_H__

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <pthread.h>

#include <curl/curl.h>

#include "download.h"

class HttpDownload : public Download {
    public:
        HttpDownload (std::string &url);
        ~HttpDownload ();

        void start_get_info ();
        void start_download ();

        void startup (const std::map<std::string,std::string> &data);
        std::map<std::string,std::string> *shutdown ();

    private:
        static void *run_info (void *download);
        static void *run_download (void *download);
        pthread_t thread;
        bool running;

        CURL *curl;
        std::ofstream ofile;

        static size_t write_function (void *ptr, size_t size, size_t nmemb, void *stream);
        static int progress_function (HttpDownload *d, double dt, double dn, double ut, double un);
};

#endif /* __HTTP_DOWNLOAD_H__ */
