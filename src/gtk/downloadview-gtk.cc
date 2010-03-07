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
#include "utils.h"

DownloadViewGtk::DownloadViewGtk (DownloadList *list) :
    DownloadView (list)
{
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new (7, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_INT, GDK_TYPE_PIXBUF);

    widget = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));

    GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (widget));
    gtk_tree_selection_set_mode (sel, GTK_SELECTION_MULTIPLE);

    renderer = gtk_cell_renderer_pixbuf_new ();
    column = gtk_tree_view_column_new_with_attributes ("", renderer, "pixbuf", 6, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);

    renderer = gtk_cell_renderer_text_new ();
    g_object_set (G_OBJECT (renderer), "ellipsize", PANGO_ELLIPSIZE_MIDDLE, "editable", TRUE, NULL);
    column = gtk_tree_view_column_new_with_attributes ("Name", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_expand (column, TRUE);
    gtk_tree_view_column_set_resizable (column, TRUE);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);
    g_signal_connect (renderer, "edited", G_CALLBACK (DownloadViewGtk::cell_edited_cb), this);

    renderer = gtk_cell_renderer_text_new ();
    g_object_set (G_OBJECT (renderer), "ellipsize", PANGO_ELLIPSIZE_MIDDLE, NULL);
    column = gtk_tree_view_column_new_with_attributes ("Status", renderer, "text", 2, NULL);
    gtk_tree_view_column_set_expand (column, TRUE);
    gtk_tree_view_column_set_resizable (column, TRUE);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);

    renderer = gtk_cell_renderer_progress_new ();
    column = gtk_tree_view_column_new_with_attributes ("Progress", renderer,
        "text", 3, "value", 4, "pulse", 5, NULL);
    gtk_tree_view_column_set_resizable (column, TRUE);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);

    g_signal_connect (widget, "button-press-event", G_CALLBACK (DownloadViewGtk::button_press_cb), this);

    std::string path = Utils::getImageResource ("download-complete.svg");
    complete = gdk_pixbuf_new_from_file_at_scale (path.c_str (), -1, 16, TRUE, NULL);

    path = Utils::getImageResource ("download-go.svg");
    go = gdk_pixbuf_new_from_file_at_scale (path.c_str (), -1, 16, TRUE, NULL);

    path = Utils::getImageResource ("download-pause.svg");
    pause = gdk_pixbuf_new_from_file_at_scale (path.c_str (), -1, 16, TRUE, NULL);

    path = Utils::getImageResource ("download-stop.svg");
    stop = gdk_pixbuf_new_from_file_at_scale (path.c_str (), -1, 16, TRUE, NULL);

    path = Utils::getImageResource ("download-yield.svg");
    yield = gdk_pixbuf_new_from_file_at_scale (path.c_str (), -1, 16, TRUE, NULL);

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

    GdkPixbuf *npb = NULL;

    if (d->get_dsize () != -1) {
        if (d->get_dsize () != 0) {
            percent = 100.0 * d->get_dtrans () / d->get_dsize ();
        }

        text = Utils::size_to_string (d->get_dtrans ()) + " / " +
               Utils::size_to_string (d->get_dsize ());
    } else {
        pulse = d->get_dtrans ();
        text = Utils::size_to_string (d->get_dtrans ());
    }

    std::string status;
    if (d->get_dspeed () > 0 && d->get_uspeed () > 0) {
        status = "U: " + Utils::size_to_string (d->get_uspeed ()) + "ps ";
        status += "D: " + Utils::size_to_string (d->get_dspeed ()) + "ps";
    } else if (d->get_dspeed () > 0) {
        status = "D: " + Utils::size_to_string (d->get_dspeed ()) + "ps";
    } else if (d->get_uspeed () > 0) {
        status = "U: " + Utils::size_to_string (d->get_uspeed ()) + "ps";
    } else {
        status = d->get_status ();
    }

    switch (d->get_state ()) {
        case STATE_PAUSED:
            npb = pause;
            break;
        case STATE_COMPLETED:
            npb = complete;
            break;
        case STATE_CONNECTING:
        case STATE_DOWNLOADING:
        case STATE_WAITING:
            npb = go;
            break;
        case STATE_QUEUED:
        case STATE_HOLDING:
            npb = yield;
            break;
        case STATE_NOT_FOUND:
        case STATE_NULL:
            npb = stop;
            break;
    };

    gtk_list_store_set (store, iter,
        0, d,
        1, d->get_name ().c_str (),
        2, status.c_str (),
        3, text.c_str (), 4, percent, 5, pulse,
        6, npb, -1);
}

void
DownloadViewGtk::list_reorder_cb (Download *d, Download *nextd)
{
    std::cout << "DownloadViewGtk::list_reorder_cb (d, nextd): stub\n";
}

gboolean
DownloadViewGtk::button_press_cb (GtkWidget *widget, GdkEventButton *event, DownloadViewGtk *dv)
{
    if (event->button == 3) {
        gboolean retval = TRUE;
        GtkTreePath *cpath;

        gtk_tree_view_get_path_at_pos (GTK_TREE_VIEW (dv->widget), event->x, event->y,
            &cpath, NULL, NULL, NULL);

        if (cpath) {
            GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (dv->widget));

            if (!gtk_tree_selection_path_is_selected (sel, cpath)) {
                retval = FALSE;
            }

            gtk_tree_path_free (cpath);
        }

        GtkWidget *menu = gtk_menu_new ();

        GtkWidget *item = gtk_image_menu_item_new_from_stock (GTK_STOCK_MEDIA_PLAY, NULL);
        gtk_menu_item_set_label (GTK_MENU_ITEM (item), "Start");
        gtk_menu_append (GTK_MENU (menu), item);
        g_signal_connect (item, "activate", G_CALLBACK (DownloadViewGtk::start_cb), dv);

        item = gtk_image_menu_item_new_from_stock (GTK_STOCK_MEDIA_PAUSE, NULL);
        gtk_menu_item_set_label (GTK_MENU_ITEM (item), "Pause");
        gtk_menu_append (GTK_MENU (menu), item);
        g_signal_connect (item, "activate", G_CALLBACK (DownloadViewGtk::pause_cb), dv);

        item = gtk_image_menu_item_new_from_stock (GTK_STOCK_REMOVE, NULL);
        gtk_menu_item_set_label (GTK_MENU_ITEM (item), "Remove");
        gtk_menu_append (GTK_MENU (menu), item);
        g_signal_connect (item, "activate", G_CALLBACK (DownloadViewGtk::remove_cb), dv);

        item = gtk_image_menu_item_new_from_stock (GTK_STOCK_DELETE, NULL);
        gtk_menu_item_set_label (GTK_MENU_ITEM (item), "Delete");
        gtk_menu_append (GTK_MENU (menu), item);
        g_signal_connect (item, "activate", G_CALLBACK (DownloadViewGtk::delete_cb), dv);

        gtk_menu_append (GTK_MENU (menu), gtk_separator_menu_item_new ());

        item = gtk_image_menu_item_new_from_stock (GTK_STOCK_DIRECTORY, NULL);
        gtk_menu_item_set_label (GTK_MENU_ITEM (item), "Open Directory");
        gtk_menu_append (GTK_MENU (menu), item);
        g_signal_connect (item, "activate", G_CALLBACK (DownloadViewGtk::open_directory_cb), dv);

        item = gtk_image_menu_item_new_from_stock (GTK_STOCK_COPY, NULL);
        gtk_menu_item_set_label (GTK_MENU_ITEM (item), "Change Directory");
        gtk_menu_append (GTK_MENU (menu), item);
        g_signal_connect (item, "activate", G_CALLBACK (DownloadViewGtk::change_directory_cb), dv);

        gtk_menu_append (GTK_MENU (menu), gtk_separator_menu_item_new ());

        item = gtk_image_menu_item_new_from_stock (GTK_STOCK_PROPERTIES, NULL);
        gtk_menu_item_set_label (GTK_MENU_ITEM (item), "Properties");
        gtk_menu_append (GTK_MENU (menu), item);
        g_signal_connect (item, "activate", G_CALLBACK (DownloadViewGtk::properties_cb), dv);

        gtk_widget_show_all (menu);

        gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL,
                        event->button, event->time);

        return retval;
    }

    return FALSE;
}

void
DownloadViewGtk::cell_edited_cb (GtkCellRenderer *renderer, char *path, char *new_text, DownloadViewGtk *dv)
{
    GtkTreeIter iter;

    if (gtk_tree_model_get_iter_from_string (GTK_TREE_MODEL (dv->store), &iter, path)) {
        Download *d;

        gtk_tree_model_get (GTK_TREE_MODEL (dv->store), &iter, 0, &d, -1);

        if (d) {
            std::string name = new_text;

            gdk_threads_leave ();
            d->set_name (name);
            gdk_threads_enter ();
        }
    }
}

gboolean
DownloadViewGtk::start_cb (GtkWidget *widget, DownloadViewGtk *dv)
{
    GtkTreeIter iter;
    Download *d;
    GList *list, *liter;

    GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (dv->widget));

    list = gtk_tree_selection_get_selected_rows (sel, NULL);

    if (list != NULL) {
        for (liter = list; liter; liter = liter->next) {
            if (gtk_tree_model_get_iter (GTK_TREE_MODEL (dv->store), &iter, (GtkTreePath*) liter->data)) {
                gtk_tree_model_get (GTK_TREE_MODEL (dv->store), &iter, 0, &d, -1);

                gdk_threads_leave ();
                dv->list->start_download (d);
                gdk_threads_enter ();
            }
        }

        g_list_foreach (list, (GFunc) gtk_tree_path_free, NULL);
        g_list_free (list);
    }
}

gboolean
DownloadViewGtk::pause_cb (GtkWidget *widget, DownloadViewGtk *dv)
{
    GtkTreeIter iter;
    Download *d;
    GList *list, *liter;

    GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (dv->widget));

    list = gtk_tree_selection_get_selected_rows (sel, NULL);

    if (list != NULL) {
        for (liter = list; liter; liter = liter->next) {
            if (gtk_tree_model_get_iter (GTK_TREE_MODEL (dv->store), &iter, (GtkTreePath*) liter->data)) {
                gtk_tree_model_get (GTK_TREE_MODEL (dv->store), &iter, 0, &d, -1);

                gdk_threads_leave ();
                dv->list->pause_download (d);
                gdk_threads_enter ();
            }
        }

        g_list_foreach (list, (GFunc) gtk_tree_path_free, NULL);
        g_list_free (list);
    }
}

gboolean
DownloadViewGtk::remove_cb (GtkWidget *widget, DownloadViewGtk *dv)
{
    GtkTreeIter iter;
    Download *d;
    GList *list, *liter;

    GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (dv->widget));

    list = gtk_tree_selection_get_selected_rows (sel, NULL);

    if (list != NULL) {
        for (liter = list; liter; liter = liter->next) {
            if (gtk_tree_model_get_iter (GTK_TREE_MODEL (dv->store), &iter, (GtkTreePath*) liter->data)) {
                gtk_tree_model_get (GTK_TREE_MODEL (dv->store), &iter, 0, &d, -1);

                gdk_threads_leave ();
                dv->list->remove_download (d);
                gdk_threads_enter ();
            }
        }

        g_list_foreach (list, (GFunc) gtk_tree_path_free, NULL);
        g_list_free (list);
    }
}

gboolean
DownloadViewGtk::delete_cb (GtkWidget *widget, DownloadViewGtk *dv)
{
    GtkTreeIter iter;
    Download *d;
    GList *list, *liter;

    GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (dv->widget));

    list = gtk_tree_selection_get_selected_rows (sel, NULL);

    if (list != NULL) {
        for (liter = list; liter; liter = liter->next) {
            if (gtk_tree_model_get_iter (GTK_TREE_MODEL (dv->store), &iter, (GtkTreePath*) liter->data)) {
                gtk_tree_model_get (GTK_TREE_MODEL (dv->store), &iter, 0, &d, -1);

                gdk_threads_leave ();
                dv->list->delete_download (d);
                gdk_threads_enter ();
            }
        }

        g_list_foreach (list, (GFunc) gtk_tree_path_free, NULL);
        g_list_free (list);
    }
}

gboolean
DownloadViewGtk::open_directory_cb (GtkWidget *widget, DownloadViewGtk *dv)
{
    std::cout << "DownloadViewGtk::open_directory_cb (): stub\n";
}

gboolean
DownloadViewGtk::change_directory_cb (GtkWidget *widget, DownloadViewGtk *dv)
{
    std::cout << "DownloadViewGtk::change_directory_cb (): stub\n";
}

gboolean
DownloadViewGtk::properties_cb (GtkWidget *widget, DownloadViewGtk *dv)
{
    std::cout << "DownloadViewGtk::properties_cb (): stub\n";
}
