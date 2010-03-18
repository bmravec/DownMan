/*
 *      browser.h
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

class Browser;

#ifndef __BROWSER_H__
#define __BROWSER_H__

#include <map>
#include <string>
#include <vector>

#include "url.h"

class Browser {
    public:
        Browser ();
        ~Browser ();

        bool load_page (const Url &url);
        bool send_post (const Url &url, std::map<std::string, std::string> &post_data);

        bool find (std::string &str, std::vector<std::string> &m);

    private:
        std::string buff;
};

#endif /* __BROWSER_H__ */
