/*
 *      downloadview.h
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

class DownloadView;

#ifndef __DOWNLOADVIEW_H__
#define __DOWNLOADVIEW_H__

#include "download.h"
#include "downloadlist.h"

class DownloadView {
    public:
        DownloadView (DownloadList *list);
        ~DownloadView ();

    protected:
        DownloadList *list;

        virtual void list_add_cb (Download *d, Download *nextd);
        virtual void list_update_cb (Download *d);
        virtual void list_remove_cb (Download *d);
        virtual void list_reorder_cb (Download *d, Download *nextd);
};

#endif /* __DOWNLOADVIEW_H__ */
