/*
 *      mainwindow.cc
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

#include "mainwindow.h"

MainWindow::MainWindow (DownMan *downman) : downman (downman)
{

}

MainWindow::~MainWindow ()
{

}

void
MainWindow::set_menubar (Menubar *menubar)
{
    printf ("MainWindow::set_menubar (menubar): stub\n");
}

void
MainWindow::set_toolbar (Toolbar *toolbar)
{
    printf ("MainWindow::set_toolbar (toolbar): stub\n");
}

void
MainWindow::set_downloadview (DownloadView *downloadview)
{
    printf ("MainWindow::set_downloadview (downloadview): stub\n");
}

void
MainWindow::set_stagingview (StagingView *stagingview)
{
    printf ("MainWindow::set_stagingview (stagingview): stub\n");
}
