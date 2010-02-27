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

#include <iostream>

#include "downman.h"

#include "application.h"
#include "mainwindow.h"
#include "gui-factory.h"

DownMan::DownMan () :
    config (Config::Instance ()),
    speedmonitor (SpeedMonitor::Instance ()),
    url_regex ("\\S*://\\S*")
{
    config.load_settings ();

    staginglist = new StagingList ();
    downloadlist = new DownloadList ();

    application = guifactory.create_application ();
    mainwindow = guifactory.create_mainwindow ();
    downloadview = guifactory.create_downloadview (downloadlist);
    stagingview = guifactory.create_stagingview (staginglist);
    toolbar = guifactory.create_toolbar ();

    mainwindow->set_downloadview (downloadview);
    mainwindow->set_stagingview (stagingview);
    mainwindow->set_toolbar (toolbar);

    toolbar->signal_add_url ().connect (sigc::mem_fun (*this, &DownMan::prompt_for_urls));
    toolbar->signal_start_staging ().connect (sigc::mem_fun (*this, &DownMan::start_staging));
    mainwindow->signal_destroy ().connect (sigc::mem_fun (*this, &DownMan::quit));

    speedmonitor.signal_update ().connect (sigc::mem_fun (*this, &DownMan::update_download));
}

DownMan::~DownMan ()
{

}

void
DownMan::setup ()
{
    std::cout << "DownMan::setup (): stub\n";

    config.load_downloads ();

    //TODO: Use returned vector to initialize downloadlist
}

void
DownMan::run ()
{
    application->run ();
}

void
DownMan::shutdown ()
{
    std::cout << "DownMan::shutdown (): stub\n";
    std::vector<std::map<std::string, std::string> > dlist;

    //TODO: Fill with downloads

    config.save (dlist);
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

        if (url_regex.find_all (urls, surls)) {
            for (int i = 0; i < surls.size (); i++) {
                Download *d = dfactory.create_download (surls[i]);
                if (d != NULL) {
                    staginglist->add_download (d, NULL);
                }
            }
        }
    }
}

void
DownMan::start_staging ()
{
    for (; staginglist->size () > 0;) {
        Download *d = staginglist->get (0);

        staginglist->remove_download (d);
        downloadlist->add_download (d, NULL);
    }
}

void
DownMan::update_download (Download *d)
{
    downloadlist->update_download (d);
}

int
main (int argc, char *argv[])
{
    DownMan *dm = new DownMan ();

    dm->setup ();
    dm->run ();
    dm->shutdown ();
}
