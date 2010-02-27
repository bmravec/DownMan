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

class GuiFactory;

#ifndef __GUI_FACTORY_H__
#define __GUI_FACTORY_H__

#include "application.h"
#include "downloadview.h"
#include "stagingview.h"
#include "mainwindow.h"
#include "toolbar.h"
#include "menubar.h"

#include "downloadlist.h"
#include "staginglist.h"


class GuiFactory {
    public:
        GuiFactory ();
        ~GuiFactory ();

        Application *create_application ();
        DownloadView *create_downloadview (DownloadList *downloadlist);
        StagingView *create_stagingview (StagingList *staginglist);
        MainWindow *create_mainwindow ();
        Toolbar *create_toolbar ();
        Menubar *create_menubar ();
};

#endif /* __GUI_FACTORY_H__ */
