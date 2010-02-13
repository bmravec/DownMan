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

#include <iostream>

#include "downloadview.h"

DownloadView::DownloadView (DownloadList *list) : list (list)
{
    if (list != NULL) {
        list->signal_add ().connect (sigc::mem_fun (*this, &DownloadView::list_add_cb));
        list->signal_update ().connect (sigc::mem_fun (*this, &DownloadView::list_update_cb));
        list->signal_remove ().connect (sigc::mem_fun (*this, &DownloadView::list_remove_cb));
        list->signal_reorder ().connect (sigc::mem_fun (*this, &DownloadView::list_reorder_cb));
    }
}

DownloadView::~DownloadView ()
{

}

void
DownloadView::list_add_cb (Download *d, Download *nextd)
{
    std::cout << "DownloadView::list_add_cb (d, nextd): stub\n";
}

void
DownloadView::list_update_cb (Download *d)
{
    std::cout << "DownloadView::list_update_cb (d): stub\n";
}

void
DownloadView::list_remove_cb (Download *d)
{
    std::cout << "DownloadView::list_remove_cb (d): stub\n";
}

void
DownloadView::list_reorder_cb (Download *d, Download *nextd)
{
    std::cout << "DownloadView::list_reorder_cb (d, nextd): stub\n";
}
