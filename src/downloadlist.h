/*
 *      downloadlist.h
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

class DownloadList;

#ifndef __DOWNLOAD_LIST_H__
#define __DOWNLOAD_LIST_H__

#include "dlist.h"

class DownloadList : public DList {
    public:
        DownloadList ();
        ~DownloadList ();

        void start_download (Download *d);
        void pause_download (Download *d);
        void remove_download (Download *d);
        void delete_download (Download *d);

    private:
        void download_update (Download *d, DownloadState state);
};

#endif /* __DOWNLOAD_LIST_H__ */
