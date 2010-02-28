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

Download::Download (Url &url, std::string match_str) :
    state (STATE_NULL), url (url), dsize (-1), dtrans (0), usize (-1),
    utrans (0), so (NULL), match_str (match_str)
{

}

Download::Download (std::string &url, std::string match_str) :
    state (STATE_NULL), url (url), dsize (-1), dtrans (0), usize (-1),
    utrans (0), so (NULL), match_str (match_str)
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
Download::startup (const std::map<std::string,std::string> &data)
{
    std::cout << "Download::startup (data): stub\n";
}

std::map<std::string,std::string>*
Download::shutdown ()
{
    std::cout << "Download::shutdown (): stub\n";

    return NULL;
}

void
Download::set_state (DownloadState state)
{
    this->state = state;

    state_changed (this, state);
}

int
Download::get_uspeed () {
    return so != NULL ? so->get_uspeed () : 0;
}

int
Download::get_dspeed () {
    return so != NULL ? so->get_dspeed () : 0;
}
