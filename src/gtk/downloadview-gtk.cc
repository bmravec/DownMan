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

#include <iostream>

#include "downloadview-gtk.h"

DownloadViewGtk::DownloadViewGtk (DownloadList *list) :
    DownloadView (list)
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
    gtk_tree_view_column_set_expand (column, TRUE);
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
DownloadViewGtk::list_add_cb (Download *d, Download *nextd)
{
    GtkTreeIter iter, iter2;
    Download *diter;

    if (nextd == NULL) {
        gtk_list_store_append (store, &iter);
        update_row (&iter, d);

    } else {
        if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), &iter)) {
            do {
                gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, 0, &diter, -1);

                if (diter == nextd) {
                    gtk_list_store_insert_before (store, &iter2, &iter);
                    update_row (&iter, d);

                    return;
                }
            } while (gtk_tree_model_iter_next (GTK_TREE_MODEL (store), &iter));

            gtk_list_store_append (store, &iter);
            update_row (&iter, d);
        } else {
            gtk_list_store_append (store, &iter);
            update_row (&iter, d);
        }
    }
}

void
DownloadViewGtk::list_update_cb (Download *d)
{
    GtkTreeIter iter;
    Download *diter;

    gdk_threads_enter ();

    if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), &iter)) {
        do {
            gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, 0, &diter, -1);

            if (diter == d) {
                update_row (&iter, d);
                break;
            }
        } while (gtk_tree_model_iter_next (GTK_TREE_MODEL (store), &iter));
    }

    gdk_threads_leave ();
}

void
DownloadViewGtk::list_remove_cb (Download *d)
{
    GtkTreeIter iter;
    Download *diter;

    if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), &iter)) {
        do {
            gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, 0, &diter, -1);

            if (diter == d) {
                gtk_list_store_remove (store, &iter);
                break;
            }
        } while (gtk_tree_model_iter_next (GTK_TREE_MODEL (store), &iter));
    }
}

void
DownloadViewGtk::update_row (GtkTreeIter *iter, Download *d)
{
    int pulse = -1;
    float percent = 0.0;
    std::string text;

    if (d->get_dsize () != -1) {
        if (d->get_dsize () != 0) {
            percent = 100.0 * d->get_dtrans () / d->get_dsize ();
        }

        text = size_to_string (d->get_dtrans ()) + " / " +
               size_to_string (d->get_dsize ());
    } else {
        pulse = d->get_dtrans ();
        text = size_to_string (d->get_dtrans ());
    }

    std::string status;
    if (d->get_dspeed () > 0 && d->get_uspeed () > 0) {
        status = "U: " + size_to_string (d->get_uspeed ()) + "ps ";
        status += "D: " + size_to_string (d->get_dspeed ()) + "ps";
    } else if (d->get_dspeed () > 0) {
        status = "D: " + size_to_string (d->get_dspeed ()) + "ps";
    } else if (d->get_uspeed () > 0) {
        status = "U: " + size_to_string (d->get_uspeed ()) + "ps";
    } else {
        status = d->get_status ();
    }

    gtk_list_store_set (store, iter,
        0, d,
        1, d->get_name ().c_str (),
        2, status.c_str (),
        3, text.c_str (), 4, percent, 5, pulse, -1);
}

void
DownloadViewGtk::list_reorder_cb (Download *d, Download *nextd)
{
    std::cout << "DownloadViewGtk::list_reorder_cb (d, nextd): stub\n";
}
