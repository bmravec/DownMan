/*
 *      downloadlist.cc
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

#include "downloadlist.h"

DownloadList::DownloadList ()
{

}

DownloadList::~DownloadList ()
{

}

void
DownloadList::download_update (Download *d, DownloadState state)
{
    if (state == STATE_NULL || state == STATE_INFO_COMPLETED || state == STATE_QUEUED) {
        d->start_download ();
    }

    update (d);
}
