/*
 *      downman.h
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

class DownMan;

#ifndef __DOWNMAN_H__
#define __DOWNMAN_H__

#include "gui-factory.h"
#include "application.h"
#include "downloadview.h"
#include "stagingview.h"
#include "mainwindow.h"
#include "toolbar.h"
#include "menubar.h"

#include "downloadlist.h"
#include "staginglist.h"
#include "speedlimit.h"

#include "download.h"

class DownMan {
    public:
        DownMan ();
        ~DownMan ();

        void setup ();
        void run ();
        void shutdown ();
        void quit ();

        void prompt_for_urls ();
    private:
        GuiFactory *guifactory;

        Application *application;
        DownloadView *downloadview;
        StagingView *stagingview;
        MainWindow *mainwindow;
        Toolbar *toolbar;
        Menubar *menubar;

        DownloadList *downloadlist;
        StagingList *staginglist;
        SpeedLimit *speedlimit;
};

#endif /* __DOWNMAN_H__ */
