/*
 *      gui-factory.cc
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

#include <stdio.h>

#include "gui-factory.h"

#include "application.h"
#include "mainwindow.h"

#include "gtk/gtk-application.h"
#include "gtk/gtk-mainwindow.h"

GuiFactory::GuiFactory (DownMan *downman)
{
    this->downman = downman;
}

GuiFactory::~GuiFactory ()
{

}

Application*
GuiFactory::create_application ()
{
    return (Application*) new GtkApplication (this->downman);
}

DownloadView *
GuiFactory::create_downloadview ()
{
    printf ("GuiFactory::create_download_view (): stub\n");

    return NULL;
}

StagingView *
GuiFactory::create_stagingview ()
{
    printf ("GuiFactory::create_staging_view (): stub\n");

    return NULL;
}

MainWindow *
GuiFactory::create_mainwindow ()
{
    return (MainWindow*) new GtkMainWindow (this->downman);
}

Toolbar *
GuiFactory::create_toolbar ()
{
    printf ("GuiFactory::create_toolbar (): stub\n");

    return NULL;
}

Menubar *
GuiFactory::create_menubar ()
{
    printf ("GuiFactory::create_menubar (): stub\n");

    return NULL;
}
