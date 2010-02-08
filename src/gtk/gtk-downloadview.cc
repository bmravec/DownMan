/*
 *      gtk-downloadview.cc
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

#include <stdio.h>

#include "gtk-downloadview.h"

GtkDownloadView::GtkDownloadView (DownMan *downman, DownloadList *downloadlist) :
    DownloadView (downman, downloadlist)
{
    widget = gtk_tree_view_new ();
}

GtkDownloadView::~GtkDownloadView ()
{

}

void
GtkDownloadView::add_download (Download *download)
{
    printf ("GtkDownloadView::add_download (download): stub\n");
}

void
GtkDownloadView::update_download (Download *download)
{
    printf ("GtkDownloadView::update_download (download): stub\n");
}

void
GtkDownloadView::remove_download (Download *download)
{
    printf ("GtkDownloadView::remove_download (download): stub\n");
}

Download**
GtkDownloadView::get_selected ()
{
    printf ("GtkDownloadView::get_selected (download): stub\n");

    return NULL;
}

GtkWidget*
GtkDownloadView::get_widget ()
{
    return widget;
}
