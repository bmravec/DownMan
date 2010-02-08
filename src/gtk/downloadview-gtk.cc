/*
 *      downloadview-gtk.cc
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

#include "downloadview-gtk.h"

DownloadViewGtk::DownloadViewGtk (DownMan *downman, DownloadList *downloadlist) :
    DownloadView (downman, downloadlist)
{
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new (6, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_INT);

    widget = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));

    GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (widget));
    gtk_tree_selection_set_mode (sel, GTK_SELECTION_MULTIPLE);

    renderer = gtk_cell_renderer_text_new ();
    g_object_set (G_OBJECT (renderer), "ellipsize", PANGO_ELLIPSIZE_MIDDLE, NULL);
    column = gtk_tree_view_column_new_with_attributes ("Name", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_expand (column, TRUE);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);

    renderer = gtk_cell_renderer_text_new ();
    g_object_set (G_OBJECT (renderer), "ellipsize", PANGO_ELLIPSIZE_MIDDLE, NULL);
    column = gtk_tree_view_column_new_with_attributes ("Status", renderer, "text", 2, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);

    renderer = gtk_cell_renderer_progress_new ();
    column = gtk_tree_view_column_new_with_attributes ("Progress", renderer,
        "text", 3, "value", 4, "pulse", 5, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);

    gtk_widget_show_all (widget);
}

DownloadViewGtk::~DownloadViewGtk ()
{

}

void
DownloadViewGtk::add_download (Download *download)
{
    printf ("DownloadViewGtk::add_download (download): stub\n");
}

void
DownloadViewGtk::update_download (Download *download)
{
    printf ("DownloadViewGtk::update_download (download): stub\n");
}

void
DownloadViewGtk::remove_download (Download *download)
{
    printf ("DownloadViewGtk::remove_download (download): stub\n");
}

Download**
DownloadViewGtk::get_selected ()
{
    printf ("DownloadViewGtk::get_selected (download): stub\n");

    return NULL;
}

GtkWidget*
DownloadViewGtk::get_widget ()
{
    return widget;
}
