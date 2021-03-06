/*
 *      browser.cc
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

#include "browser.h"
#include "http-connection.h"
#include "dregex.h"

Browser::Browser ()
{

}

Browser::~Browser ()
{
}

bool
Browser::load_page (const Url &url)
{
    HttpConnection conn (url.get_host (), url.get_port ());

    buff.clear ();

    if (conn.send_get_request (url.get_path ())) {
        char *tbuff = new char[5120];

        int len;
        while ((len = conn.read (tbuff, 5120)) > 0) {
            buff.append (tbuff, len);
        }

        delete tbuff;

        return true;
    } else {
        return false;
    }
}

bool
Browser::send_post (const Url &url, std::map<std::string,std::string> &post_data)
{
    HttpConnection conn (url.get_host (), url.get_port ());

    buff.clear ();

    if (conn.send_post_request (url.get_path (), post_data)) {
        char *tbuff = new char[5120];

        int len;
        while ((len = conn.read (tbuff, 5120)) > 0) {
            buff.append (tbuff, len);
        }

        delete tbuff;

        return true;
    } else {
        return false;
    }
}

bool
Browser::find (const std::string &str, std::vector<std::string> &m)
{
    DRegex regex (str);

    return regex.find (buff, m);
}
