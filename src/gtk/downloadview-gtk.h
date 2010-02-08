/*
 *      downloadview-gtk.h
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

class DownloadViewGtk;

#ifndef __DOWNLOAD_VIEW_GTK_H__
#define __DOWNLOAD_VIEW_GTK_H__

#include <gtk/gtk.h>

#include "download.h"
#include "downloadlist.h"
#include "downloadview.h"
#include "downman.h"

class DownloadViewGtk : public DownloadView {
    public:
        DownloadViewGtk (DownMan *downman, DownloadList *downloadlist);
        ~DownloadViewGtk ();

        void add_download (Download *download);
        void update_download (Download *download);
        void remove_download (Download *download);
        Download **get_selected ();

        GtkWidget *get_widget ();

    protected:
        DownloadList *downloadlist;
        DownMan *downman;

        GtkWidget *widget;
        GtkListStore *store;
};

#endif /* __DOWNLOAD_VIEW_GTK_H__ */
