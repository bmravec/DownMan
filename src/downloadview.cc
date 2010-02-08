/*
 *      downloadview.cc
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

#include "downloadview.h"

DownloadView::DownloadView (DownMan *downman, DownloadList *downloadlist) :
    downloadlist (downloadlist),
    downman (downman)
{

}

DownloadView::~DownloadView ()
{

}

void
DownloadView::add_download (Download *download)
{
    printf ("DownloadView::add_download (download): stub\n");
}

void
DownloadView::update_download (Download *download)
{
    printf ("DownloadView::update_download (download): stub\n");
}

void
DownloadView::remove_download (Download *download)
{
    printf ("DownloadView::remove_download (download): stub\n");
}

Download**
DownloadView::get_selected ()
{
    printf ("DownloadView::get_selected (download): stub\n");

    return NULL;
}
