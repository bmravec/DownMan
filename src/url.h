/*
 *      url.h
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

class Url;

#ifndef __URL_H__
#define __URL_H__

#include <string>

class Url {
    public:
        Url (std::string &url);
        Url (const char *url);
        Url ();
        ~Url ();

        Url &operator= (std::string &url);
        Url &operator= (Url &url);

        void parse_url (std::string &url);
        void parse_url (const char *url);

        std::string &get_url () { return url; }
        std::string &get_host () { return host; }
        int get_port () { return port; }
        std::string &get_proto () { return proto; }
        std::string &get_path () { return path; }
        std::string &get_name () { return name; }
        std::string &get_ext () { return ext; }

    private:
        std::string url, host, proto, path, name, ext;
        int port;
};

#endif /* __URL_H__ */
