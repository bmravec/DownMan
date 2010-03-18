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
        Url ();
        Url (const std::string &url);
        ~Url ();

        Url &operator= (const std::string &url);
        Url &operator= (const Url &url);

        void parse_url (const std::string &url);

        std::string get_url () const { return url; }
        std::string get_host () const { return host; }
        int get_port () const { return port; }
        std::string get_proto () const { return proto; }
        std::string get_path () const { return path; }
        std::string get_name () const { return name; }
        std::string get_ext () const { return ext; }

        static std::string encode (const std::string &url);
        static std::string decode (const std::string &url);

    private:
        std::string url, host, proto, path, name, ext;
        int port;
};

#endif /* __URL_H__ */
