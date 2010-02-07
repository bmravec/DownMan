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

#ifndef __DOWNMAN_H__
#define __DOWNMAN_H__

class GuiFactory;
class Application;
class DownloadView;
class StagingView;
class MainWindow;
class Toolbar;
class Menubar;
class DownloadList;
class StagingList;
class SpeedLimit;

class DownMan {
    public:
        DownMan ();
        ~DownMan ();

        void setup ();
        void run ();
        void shutdown ();
        void quit ();
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
