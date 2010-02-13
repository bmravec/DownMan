/*
 *      mainwindow-gtk.h
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

class MainWindowGtk;

#ifndef __MAIN_WINDOW_GTK_H__
#define __MAIN_WINDOW_GTK_H__

#include <gtk/gtk.h>

#include "downloadview.h"
#include "downloadview-gtk.h"
#include "downman.h"
#include "mainwindow.h"
#include "menubar.h"
#include "stagingview.h"
#include "stagingview-gtk.h"
#include "toolbar.h"
#include "toolbar-gtk.h"

class MainWindowGtk : public MainWindow {
    public:
        MainWindowGtk (DownMan *downman);
        ~MainWindowGtk ();

        void set_menubar (Menubar *menubar);
        void set_toolbar (Toolbar *toolbar);
        void set_downloadview (DownloadView *downloadview);
        void set_stagingview (StagingView *stagingview);

        static void destroy_cb (GtkWidget *widget, MainWindowGtk *mw);

    private:
        GtkWidget *window;
        GtkWidget *table;
        GtkWidget *notebook;

        GtkWidget *downloadview_sw, *stagingview_sw;

        DownloadViewGtk *downloadview;
        StagingViewGtk *stagingview;
        ToolbarGtk *toolbar;
};

#endif /* __MAIN_WINDOW_GTK_H__ */
