/*
 *      gtk-mainwindow.h
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

#ifndef __GTK_MAINWINDOW_H__
#define __GTK_MAINWINDOW_H__

class DownMan;
class Menubar;
class Toolbar;
class DownloadView;
class StagingView;

#include "mainwindow.h"

#include <gtk/gtk.h>

class GtkMainWindow : public MainWindow {
    public:
        GtkMainWindow (DownMan *downman);
        ~GtkMainWindow ();

        void set_menubar (Menubar *menubar);
        void set_toolbar (Toolbar *toolbar);
        void set_downloadview (DownloadView *downloadview);
        void set_stagingview (StagingView *stagingview);

        static void destroy_cb (GtkWidget *widget, GtkMainWindow *mw);
    private:
        GtkWidget *window;
};

#endif /* __GTK_MAINWINDOW_H__ */
