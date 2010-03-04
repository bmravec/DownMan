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

    private:
        void download_update (Download *d, DownloadState state);

        void download_view_start (Download *d);
        void download_view_pause (Download *d);
        void download_view_remove (Download *d);
        void download_view_delete (Download *d);
};

#endif /* __DOWNLOAD_LIST_H__ */
