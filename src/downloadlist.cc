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

#include <iostream>

#include "downloadlist.h"
#include "utils.h"

DownloadList::DownloadList ()
{

}

DownloadList::~DownloadList ()
{

}

void
DownloadList::download_update (Download *d, DownloadState state)
{
    if (state == STATE_CONNECTING || state == STATE_DOWNLOADING) {
        update (d);
        return;
    }

    if (state == STATE_NULL || state == STATE_INFO_COMPLETED) {
        d->queue ();
        return;
    }

    std::vector<Download*>::iterator iter;

    int limit = Utils::parseInt (Config::Instance ().get_property (Config::MAX_NUM_DOWNLOADS));
    int total_active = 0;

    for (iter = downloads.begin (); iter != downloads.end (); iter++) {
        DownloadState ds = (*iter)->get_state ();

        if (ds == STATE_CONNECTING || ds == STATE_DOWNLOADING || ds == STATE_WAITING) {
            total_active++;
        }
    }

    for (iter = downloads.begin (); iter != downloads.end () && total_active < limit; iter++) {
        DownloadState ds = (*iter)->get_state ();

        if (ds == STATE_QUEUED) {
            (*iter)->start_download ();
            total_active++;
        }
    }

    update (d);
}

void
DownloadList::start_download (Download *d)
{
    if (d->get_state () == STATE_PAUSED) {
        d->queue ();
    }
}

void
DownloadList::pause_download (Download *d)
{
    DownloadState ds = d->get_state ();
    if (ds == STATE_CONNECTING || ds == STATE_DOWNLOADING || ds == STATE_WAITING) {
        d->pause ();
    }
}

void
DownloadList::delete_download (Download *d)
{
    std::cout << "DownloadList::Delete download: " << d->get_name () << std::endl;
    //TODO: Delete from hard disk

    remove_download (d);
}
