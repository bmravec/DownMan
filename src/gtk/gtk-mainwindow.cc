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
#include "gtk-downloadview.h"

GtkMainWindow::GtkMainWindow (DownMan *downman) : MainWindow (downman)
{
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);

    gtk_window_set_title (GTK_WINDOW (window), PACKAGE_NAME " v" PACKAGE_VERSION);
    gtk_window_set_icon_from_file (GTK_WINDOW (window),
        SHARE_DIR "/imgs/downman.svg", NULL);

    table = gtk_table_new (4, 1, FALSE);

    notebook = gtk_notebook_new ();

    downloadview_sw = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (downloadview_sw),
        GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_notebook_insert_page (GTK_NOTEBOOK (notebook), downloadview_sw,
        gtk_label_new ("Downloads"), 0);

    stagingview_sw = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (stagingview_sw),
        GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_notebook_insert_page (GTK_NOTEBOOK (notebook), stagingview_sw,
        gtk_label_new ("Staging"), 1);

    gtk_table_attach (GTK_TABLE (table), notebook, 0, 1 ,2, 3,
        (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
        (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

    gtk_container_add (GTK_CONTAINER (window), table);

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
    this->downloadview = (GtkDownloadView*) downloadview;

    gtk_container_add (GTK_CONTAINER (downloadview_sw), this->downloadview->get_widget ());
    gtk_widget_show_all (downloadview_sw);
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
