/*
 *      staginglist.cc
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

#include "staginglist.h"

StagingList::StagingList ()
{

}

StagingList::~StagingList ()
{

}

void
StagingList::add_download (Download *d, Download *nextd)
{
    std::vector<Download*>::iterator iter;
    Download *diter = NULL;

    d->start_get_info ();

    if (nextd == NULL) {
        for (iter = downloads.begin (); iter < downloads.end (); iter++) {
            if (*iter == nextd) {
                downloads.insert (iter, d);
                diter = *iter;
                break;
            }
        }
    }

    if (diter != NULL) {
        downloads.push_back (d);
    }

    d->signal_state_changed ().connect (sigc::mem_fun (*this, &StagingList::download_update));

    add (d, diter);
}

void
StagingList::update_download (Download *d)
{
    update (d);
}

void
StagingList::remove_download (Download *d)
{
    std::vector<Download*>::iterator iter;

    for (iter = downloads.begin (); iter < downloads.end (); iter++) {
        if (*iter == d) {
            downloads.erase (iter);
            remove (d);
            break;
        }
    }
}

void
StagingList::reorder_download (Download *d, Download *nextd)
{
    std::vector<Download*>::iterator iter;
    Download *diter = NULL;

    for (iter = downloads.begin (); iter < downloads.end (); iter++) {
        if (*iter == d) {
            downloads.erase (iter);
            break;
        }
    }

    if (nextd == NULL) {
        for (iter = downloads.begin (); iter < downloads.end (); iter++) {
            if (*iter == nextd) {
                downloads.insert (iter, d);
                diter = *iter;
                break;
            }
        }
    }

    if (diter != NULL) {
        downloads.push_back (d);
    }

    reorder (d, nextd);
}

void
StagingList::download_update (Download *d, DownloadState state)
{
    update_download (d);
}
