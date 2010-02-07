/*
 *      gui-factory.h
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

#ifndef __GUI_FACTORY_H__
#define __GUI_FACTORY_H__

class DownMan;
class Application;
class DownloadView;
class StagingView;
class MainWindow;
class Toolbar;
class Menubar;

class GuiFactory {
    public:
        GuiFactory (DownMan *downman);
        ~GuiFactory ();

        Application *create_application ();
        DownloadView *create_downloadview ();
        StagingView *create_stagingview ();
        MainWindow *create_mainwindow ();
        Toolbar *create_toolbar ();
        Menubar *create_menubar ();
    private:
        DownMan *downman;
};

#endif /* __GUI_FACTORY_H__ */
