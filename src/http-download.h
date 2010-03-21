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

#include <fstream>
#include <string>
#include <map>
#include <pthread.h>
#include <vector>

#include "download.h"
#include "dregex.h"

class HttpDownload : public Download {
    public:
        HttpDownload ();
        HttpDownload (std::vector<std::string> &m);
        ~HttpDownload ();

        void start_get_info ();
        void start_download ();
        void pause ();

        bool startup (std::map<std::string,std::string> &data);
        bool shutdown (std::map<std::string, std::string> &data);

        static Download *create (std::vector<std::string> &m) { return new HttpDownload (m); }
        static Download *create () { return new HttpDownload (); }
        static const DRegex MATCH_REGEX;

        static const std::string KEY_LOCAL_PATH;
        static const std::string KEY_REMOTE_PATH;
        static const std::string KEY_REMOTE_HOST;
        static const std::string KEY_REMOTE_PORT;

    private:
        void run_info ();
        void run_download ();

        std::string local_path;
        std::string remote_host;
        int remote_port;
        std::string remote_path;

        static void *static_run_info (void *download);
        static void *static_run_download (void *download);

        pthread_t thread;
        bool running;

        std::ofstream ofile;
};

#endif /* __HTTP_DOWNLOAD_H__ */
