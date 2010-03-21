/*
 *      download-objects.h
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

#include "http-download.h"
#include "ftp-download.h"

typedef Download* (*DownloadCreateFuncVec) (std::vector<std::string>&);
typedef Download* (*DownloadCreateFunc) ();
typedef struct {
    const DRegex *regex;
    DownloadCreateFuncVec func_vec;
    DownloadCreateFunc func;
} DownloadCreatePair;

static DownloadCreatePair dos [] = {
    { &HttpDownload::MATCH_REGEX, HttpDownload::create, HttpDownload::create },
    { &FtpDownload::MATCH_REGEX, FtpDownload::create, FtpDownload::create },
    { NULL, NULL, NULL},
};
