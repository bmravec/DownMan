/*
 *      dlist.cc
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

#include "dlist.h"

DList::DList ()
{

}

DList::~DList ()
{

}

void
DList::add_download (Download *d, Download *nextd)
{
    std::vector<Download*>::iterator iter;
    Download *diter = NULL;

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

    d->signal_state_changed ().connect (sigc::mem_fun (*this, &DList::download_update));

    add (d, diter);

    update_download (d);
}

void
DList::update_download (Download *d)
{
    download_update (d, d->get_state ());
}

void
DList::remove_download (Download *d)
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
DList::reorder_download (Download *d, Download *nextd)
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
DList::download_update (Download *d, DownloadState state)
{
    update (d);
}
