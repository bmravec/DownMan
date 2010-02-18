/*
 *      dlist.h
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

class DList;

#ifndef __DLIST_H__
#define __DLIST_H__

#include <sigc++/sigc++.h>
#include <vector>

#include "download.h"

class DList {
    public:
        DList ();
        ~DList ();

        void add_download (Download *d, Download *nextd);
        void update_download (Download *d);
        void remove_download (Download *d);

        Download *get (int index) { return downloads[index]; }
        int size () const { return downloads.size (); }

        void reorder_download (Download *d, Download *nextd);

        sigc::signal<void, Download*, Download*> &signal_add () { return add; }
        sigc::signal<void, Download*> &signal_update () { return update; }
        sigc::signal<void, Download*> &signal_remove () { return remove; }

        sigc::signal<void, Download*, Download*> &signal_reorder () { return reorder; }

    protected:
        sigc::signal<void, Download*, Download*> add;
        sigc::signal<void, Download*> update;
        sigc::signal<void, Download*> remove;

        sigc::signal<void, Download*, Download*> reorder;

        std::vector<Download*> downloads;

        virtual void download_update (Download *d, DownloadState state);
};

#endif /* __DLIST_H__ */
