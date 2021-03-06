/*
 *      url.cc
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

#include "url.h"
#include "dregex.h"
#include "utils.h"

Url::Url () : port (0)
{

}

Url::Url (const std::string &url) : port (0)
{
    parse_url (url);
}

Url::~Url ()
{

}

Url&
Url::operator= (const std::string &url)
{
    parse_url (url);

    return *this;
}

Url&
Url::operator= (const Url &url)
{
    this->url = url.url;
    host = url.host;
    proto = url.proto;
    path = url.path;
    name = url.name;
    ext = url.ext;
    port = url.port;

    return *this;
}

void
Url::parse_url (const std::string &url)
{
    DRegex regex ("(\\S*)://([^/ \t\r\n\f\v]*)(\\S*)");

    std::vector<std::string> results;
    size_t pos;

    this->url = "";
    proto = "";
    host = "";
    port = 0;
    path = "";
    name = "";
    ext = "";

    if (regex.find (url, results)) {
        this->url = results[0];
        proto = results[1];
        host = results[2];

        pos = host.find (":");
        if (pos != std::string::npos) {
            port = Utils::parseInt (host.c_str () + pos + 1);
            host = host.substr (0, pos);
        }

        path = results[3];
        pos = path.rfind ("/");
        if (pos != std::string::npos) {
            name = path.substr (pos + 1);
        }

        pos = name.rfind (".");
        if (name.size () > 0 && pos != std::string::npos) {
            ext = name.substr (pos + 1);
        }
    }
}

std::string
Url::encode (const std::string &url)
{
    std::string str;

    for (int i = 0; i < url.size (); i++) {
        if ((url[i] >= '0' && url[i] <= '9') ||
            (url[i] >= 'A' && url[i] <= 'Z') ||
            (url[i] >= 'a' && url[i] <= 'z')) {
            str += url[i];
        } else {
            if (url[i] & 0xff < 16) {
                str += "%0" + Utils::formatHexInt (url[i] & 0xff);
            } else {
                str += "%" + Utils::formatHexInt (url[i] & 0xff);
            }
        }
    }

    return str;
}

int
get_hex_digit (char b)
{
    if (b >= '0' && b <= '9') {
        return b - '0';
    } else if (b >= 'A' && b <= 'F') {
        return 10 + b - 'A';
    } else if (b >= 'a' && b <= 'f') {
        return 10 + b - 'a';
    }

    return 0;
}

std::string
Url::decode (const std::string &url)
{
    std::string str;

    for (int i = 0; i < url.size (); i++) {
        if (url[i] == '%') {
            int val = 0;
            int tmp;
            if ((tmp = get_hex_digit (url[i+1])) != -1) {
                val += 16 * tmp;
            } else {
               return "";
            }

            if ((tmp = get_hex_digit (url[i+2])) != -1) {
                val += tmp;
            } else {
               return "";
            }

            str += (char) val;
            i += 2;
        } else if (url[i] == '+') {
            str += ' ';
        } else {
            str += url[i];
        }
    }

    return str;
}
