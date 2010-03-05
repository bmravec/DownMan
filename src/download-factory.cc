/*
 *      download-factory.cc
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

#include "download-factory.h"

#include "http-download.h"
#include "ftp-download.h"

DownloadFactory::DownloadFactory ()
{

}

DownloadFactory::~DownloadFactory ()
{

}

Download*
DownloadFactory::create_download (Url &url)
{
    Download *d = NULL;

    if (url.get_proto () == "http") {
        d = new HttpDownload (url);
    } else if (url.get_proto () == "ftp") {
        d = new FtpDownload (url);
    }

    return d;
}

Download*
DownloadFactory::build_download (std::map<std::string, std::string> &data)
{
    Download *d = NULL;

    if (data[Download::KEY_MATCH] == "http") {
        d = new HttpDownload ();
    } else if (data[Download::KEY_MATCH] == "ftp") {
        d = new FtpDownload ();
    }

    if (d != NULL) {
        if (d->startup (data)) {
            return d;
        } else {
            delete d;
            return NULL;
        }
    }

    return d;
}
