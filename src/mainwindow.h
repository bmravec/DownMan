/*
 *      mainwindow.h
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

class MainWindow;

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <sigc++/sigc++.h>

#include "menubar.h"
#include "toolbar.h"
#include "downloadview.h"
#include "stagingview.h"

class MainWindow {
    public:
        MainWindow ();
        ~MainWindow ();

        virtual void set_menubar (Menubar *menubar);
        virtual void set_toolbar (Toolbar *toolbar);
        virtual void set_downloadview (DownloadView *downloadview);
        virtual void set_stagingview (StagingView *stagingview);

        sigc::signal<void> &signal_destroy () { return destroy; }

    protected:
        sigc::signal<void> destroy;
};

#endif /* __MAIN_WINDOW_H__ */
