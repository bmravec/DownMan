/*
 *      gtk-mainwindow.cc
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

#include "../config.h"

#include <stdio.h>

#include "downman.h"
#include "gtk-mainwindow.h"

GtkMainWindow::GtkMainWindow (DownMan *downman) : MainWindow (downman)
{
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gchar *str = g_strdup_printf ("%s v%s", PACKAGE_NAME, PACKAGE_VERSION);
    gtk_window_set_title (GTK_WINDOW (window), str);
    g_free (str);

    gchar *path = g_strdup_printf ("%s/imgs/downman.svg", SHARE_DIR);
    gtk_window_set_icon_from_file (GTK_WINDOW (window), path, NULL);
    g_free (path);

    g_signal_connect (window, "destroy", G_CALLBACK (destroy_cb), this);

    gtk_widget_show_all (window);
}

GtkMainWindow::~GtkMainWindow ()
{

}

void
GtkMainWindow::set_menubar (Menubar *menubar)
{
    printf ("GtkMainWindow::set_menubar (menubar): stub\n");
}

void
GtkMainWindow::set_toolbar (Toolbar *toolbar)
{
    printf ("GtkMainWindow::set_toolbar (toolbar): stub\n");
}

void
GtkMainWindow::set_downloadview (DownloadView *downloadview)
{
    printf ("GtkMainWindow::set_downloadview (downloadview): stub\n");
}

void
GtkMainWindow::set_stagingview (StagingView *stagingview)
{
    printf ("GtkMainWindow::set_stagingview (stagingview): stub\n");
}

void
GtkMainWindow::destroy_cb (GtkWidget *widget, GtkMainWindow *mw)
{
    mw->downman->quit ();
}
