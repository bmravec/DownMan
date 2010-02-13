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
#include <iostream>

#include "downman.h"

#include "application.h"
#include "mainwindow.h"
#include "gui-factory.h"

#include "http-download.h"

DownMan::DownMan ()
{
    guifactory = new GuiFactory (this);

    staginglist = new StagingList ();

    application = guifactory->create_application ();
    mainwindow = guifactory->create_mainwindow ();
    downloadview = guifactory->create_downloadview (NULL);
    stagingview = guifactory->create_stagingview (staginglist);
    toolbar = guifactory->create_toolbar ();

    mainwindow->set_downloadview (downloadview);
    mainwindow->set_stagingview (stagingview);
    mainwindow->set_toolbar (toolbar);

    toolbar->signal_add_url ().connect (sigc::mem_fun (*this, &DownMan::prompt_for_urls));

    url_regex = new  DRegex ("\\S*://\\S*");
}

DownMan::~DownMan ()
{
    delete url_regex;
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

void
DownMan::prompt_for_urls ()
{
    std::string urls = application->prompt_for_urls ();

    if (urls.length () > 0) {
        std::vector<std::string> surls;

        if (url_regex->find_all (urls, surls)) {
            for (int i = 0; i < surls.size (); i++) {
                HttpDownload *d = new HttpDownload (surls[i]);
                staginglist->add_download (d, NULL);
            }
        }
    } else {
        std::cout << "No URLs Entered\n";
    }
}

int
main (int argc, char *argv[])
{
    DownMan *dm = new DownMan ();

    dm->setup ();
    dm->run ();
    dm->shutdown ();
}
