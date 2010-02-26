/*
 *      stagingview.cc
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

#include "stagingview-gtk.h"
#include "utils.h"

StagingViewGtk::StagingViewGtk (StagingList *staginglist) :
    StagingView (staginglist)
{
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new (4, G_TYPE_POINTER, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_STRING);

    widget = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));

    GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (widget));
    gtk_tree_selection_set_mode (sel, GTK_SELECTION_MULTIPLE);

    renderer = gtk_cell_renderer_text_new ();
    g_object_set (G_OBJECT (renderer), "ellipsize", PANGO_ELLIPSIZE_MIDDLE, NULL);
    column = gtk_tree_view_column_new_with_attributes ("Name", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_expand (column, TRUE);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);

    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Size", renderer, "text", 2, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);

    renderer = gtk_cell_renderer_text_new ();
    g_object_set (G_OBJECT (renderer), "ellipsize", PANGO_ELLIPSIZE_MIDDLE, NULL);
    column = gtk_tree_view_column_new_with_attributes ("Status", renderer, "text", 3, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);

    gtk_widget_show_all (widget);
}

StagingViewGtk::~StagingViewGtk ()
{

}

void
StagingViewGtk::list_add_cb (Download *d, Download *nextd)
{
    Download *diter;
    std::string size;

    if (d->get_dsize () > 0) {
        size = Utils::size_to_string (d->get_dsize ());
    } else {
        size = "Unknown";
    }

    if (nextd == NULL) {
        gtk_list_store_insert_with_values (store, NULL, -1,
            0, d,
            1, d->get_name ().c_str (),
            2, size.c_str (),
            3, d->get_status ().c_str (), -1);
    } else {
        GtkTreeIter iter, iter2;
        if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), &iter)) {
            do {
                gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, 0, &diter, -1);

                if (diter == nextd) {
                    gtk_list_store_insert_before (store, &iter2, &iter);
                    gtk_list_store_set (store, &iter,
                        0, d,
                        1, d->get_name ().c_str (),
                        2, size.c_str (),
                        3, d->get_status ().c_str (), -1);
                    return;
                }
            } while (gtk_tree_model_iter_next (GTK_TREE_MODEL (store), &iter));

            gtk_list_store_append (store, &iter);
            gtk_list_store_set (store, &iter,
                0, d,
                1, d->get_name ().c_str (),
                2, size.c_str (),
                3, d->get_status ().c_str (), -1);
        } else {
            gtk_list_store_append (store, &iter);
            gtk_list_store_set (store, &iter,
                0, d,
                1, d->get_name ().c_str (),
                2, size.c_str (),
                3, d->get_status ().c_str (), -1);
        }
    }
}

void
StagingViewGtk::list_update_cb (Download *d)
{
    GtkTreeIter iter;
    Download *diter;
    std::string size;

    if (d->get_dsize () > 0) {
        size = Utils::size_to_string (d->get_dsize ());
    } else {
        size = "Unknown";
    }

    if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), &iter)) {
        do {
            gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, 0, &diter, -1);

            if (diter == d) {
                gtk_list_store_set (store, &iter,
                    1, d->get_name ().c_str (),
                    2, size.c_str (),
                    3, d->get_status ().c_str (), -1);
            }
        } while (gtk_tree_model_iter_next (GTK_TREE_MODEL (store), &iter));
    }
}

void
StagingViewGtk::list_remove_cb (Download *d)
{
    GtkTreeIter iter;
    Download *diter;

    if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), &iter)) {
        do {
            gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, 0, &diter, -1);

            if (diter == d) {
                gtk_list_store_remove (store, &iter);
            }
        } while (gtk_tree_model_iter_next (GTK_TREE_MODEL (store), &iter));
    }
}

void
StagingViewGtk::list_reorder_cb (Download *d, Download *nextd)
{
    std::cout << "StagingViewGtk::list_reorder_cb (d, nextd): stub\n";
}
