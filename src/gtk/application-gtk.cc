/*
 *      application-gtk.cc
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

#include <gtk/gtk.h>

#include "application-gtk.h"

GStaticRecMutex rmutex;

void
lock_function () {
    g_static_rec_mutex_lock (&rmutex);
}

void
unlock_function () {
    g_static_rec_mutex_unlock (&rmutex);
}

ApplicationGtk::ApplicationGtk ()
{
    g_type_init ();

    g_static_rec_mutex_init (&rmutex);

    gdk_threads_set_lock_functions (G_CALLBACK (lock_function),
        G_CALLBACK (unlock_function));

    g_thread_init (NULL);
    gdk_threads_init ();

    gint argc = 0;
    gtk_init (&argc, NULL);
}

ApplicationGtk::~ApplicationGtk ()
{

}

void
ApplicationGtk::run ()
{
    gdk_threads_enter ();
    gtk_main ();
    gdk_threads_leave ();
}

void
ApplicationGtk::quit ()
{
    gtk_main_quit ();
}

std::string
ApplicationGtk::prompt_for_captcha (char *img_data)
{
    std::string text;

    std::cout << "ApplicationGtk::prompt_for_captcha (): stub\n";

    return text;
}

std::string
ApplicationGtk::prompt_for_urls ()
{
    std::string text;

    GtkWidget *tv = gtk_text_view_new ();

    GtkWidget *dialog = gtk_dialog_new_with_buttons ("Add URLs...", NULL,
        GTK_DIALOG_MODAL, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
        GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

    GtkWidget *vbox = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

    gtk_box_pack_start (GTK_BOX (vbox), gtk_label_new ("Enter URLs below:"), FALSE, FALSE, 0);

    GtkWidget *sw = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER (sw), tv);

    gtk_box_pack_start (GTK_BOX (vbox), sw, TRUE, TRUE, 0);

    gtk_widget_show_all (vbox);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK) {
        GtkTextIter start, end;

        GtkTextBuffer *buff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));

        gtk_text_buffer_get_start_iter (buff, &start);
        gtk_text_buffer_get_end_iter (buff, &end);

        gchar *gtext = gtk_text_buffer_get_text (buff, &start, &end, TRUE);

        text.append (gtext);

        g_free (gtext);
    }

    gtk_widget_destroy (dialog);

    return text;
}

std::string
ApplicationGtk::prompt_for_files ()
{
    std::string text;

    std::cout << "ApplicationGtk::prompt_for_files (): stub\n";

    return text;
}
