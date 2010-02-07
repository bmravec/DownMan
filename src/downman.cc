/*
 *      downman.cc
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

#include "downman.h"
#include "application.h"
#include "gui-factory.h"

DownMan::DownMan ()
{
    guifactory = new GuiFactory (this);

    application = guifactory->create_application ();
    mainwindow = guifactory->create_mainwindow ();
}

DownMan::~DownMan ()
{

}

void
DownMan::setup ()
{
    printf ("DownMan::setup (): stub\n");
}

void
DownMan::run ()
{
    application->run ();
}

void
DownMan::shutdown ()
{
    printf ("DownMan::shutdown (): stub\n");
}

void
DownMan::quit ()
{
    application->quit ();
}

int
main (int argc, char *argv[])
{
    DownMan *dm = new DownMan ();

    dm->setup ();
    dm->run ();
    dm->shutdown ();
}
