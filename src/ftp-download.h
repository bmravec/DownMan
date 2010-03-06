/*
 *      ftp-download.h
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

class FtpDownload;

#ifndef __FTP_DOWNLOAD_H__
#define __FTP_DOWNLOAD_H__

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <pthread.h>

#include "download.h"
#include "url.h"

class FtpDownload : public Download {
    public:
        FtpDownload ();
        FtpDownload (Url &url);
        ~FtpDownload ();

        void start_get_info ();
        void start_download ();

        bool startup (std::map<std::string,std::string> &data);
        bool shutdown (std::map<std::string, std::string> &data);

    private:
        void run_info ();
        void run_download ();

        static void *static_run_info (void *download);
        static void *static_run_download (void *download);

        pthread_t thread;
        bool running;

        std::ofstream ofile;
};

#endif /* __FTP_DOWNLOAD_H__ */
