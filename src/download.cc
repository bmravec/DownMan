/*
 *      download.cc
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

#include "download.h"

const std::string Download::KEY_DISPLAY_NAME ("display-name");
const std::string Download::KEY_DOWNLOADED ("downloaded");
const std::string Download::KEY_SIZE ("size");
const std::string Download::KEY_STATE ("state");
const std::string Download::KEY_LOCATION ("location");
const std::string Download::KEY_MATCH ("match");

Download::Download () :
    state (STATE_NULL), so (NULL),
    dsize (-1), dtrans (0), usize (-1), utrans (0)
{

}

Download::~Download ()
{

}

void
Download::start_get_info ()
{
    set_state (STATE_NOT_FOUND);
}

void
Download::start_download ()
{
    set_state (STATE_NOT_FOUND);
}

void
Download::delete_data ()
{

}

void
Download::pause ()
{
    set_state (STATE_PAUSED);
}

void
Download::queue ()
{
    set_state (STATE_QUEUED);
}

bool
Download::startup (std::map<std::string,std::string> &data)
{
    std::cout << "Download::startup (data): stub\n";

    return false;
}

bool
Download::shutdown (std::map<std::string,std::string> &data)
{
    std::cout << "Download::shutdown (): stub\n";

    return false;
}

void
Download::set_state (DownloadState state)
{
    this->state = state;

    state_changed (this, state);
}

int
Download::get_uspeed () const {
    return so != NULL ? so->get_uspeed () : 0;
}

int
Download::get_dspeed () const {
    return so != NULL ? so->get_dspeed () : 0;
}
