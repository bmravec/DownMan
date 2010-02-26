/*
 *      http-connection.cc
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
#include <fstream>

#include "http-connection.h"
#include "utils.h"

#define BUFFER_TOTAL 51200

HttpConnection::HttpConnection () :
    url (NULL), socket (NULL), buff (new char [BUFFER_TOTAL]), bufflen (0)
{

}

HttpConnection::~HttpConnection ()
{
    delete buff;
}

bool
HttpConnection::send_get_request (Url &url, const char *range)
{
    if (socket == NULL) {
        socket = new Socket (url.get_host ().c_str (), url.get_port () != 0 ? url.get_port () : 80);

        std::string request = "GET " + url.get_path () + " HTTP/1.1\n";
        request += "Host: " + url.get_host () + "\n";
        request += "\n";

        socket->write (request);

        return read_header ();
    } else {
        return false;
    }
}

bool
HttpConnection::send_head_request (Url &url)
{
    if (socket == NULL) {
        socket = new Socket (url.get_host ().c_str (), url.get_port () != 0 ? url.get_port () : 80);

        std::string request = "HEAD " + url.get_path () + " HTTP/1.1\n";
        request += "Host: " + url.get_host () + "\n";
        request += "\n";

        socket->write (request);

        return read_header ();
    } else {
        return false;
    }
}

bool
HttpConnection::send_post_request (Url &url, const char *post_data)
{
    return false;
}

bool
HttpConnection::read_header ()
{
    size_t lbegin, lend, split;

    bufflen = socket->read (buff, BUFFER_TOTAL);
    buffpos = 0;

    header.clear ();

    version = std::string (buff, 8);
    response_code = Utils::parseInt (buff + 9);
    for (lend = 0; !(buff[lend] == '\n' || (lend != 0 && buff[lend - 1] == '\n' && buff[lend] == '\r')); lend++);
    code = std::string (buff + 13, lend - 14);

    std::cout << "Version: " << version << std::endl;
    std::cout << "Code: " << response_code << std::endl;
    std::cout << "Code: " << code << std::endl;

    for (lbegin = lend = 0; true; lend++) {
        if (buff[lend] == '\n' || (lend != 0 && buff[lend - 1] == '\r' && buff[lend] == '\n')) {
            if ((lend != 0 && buff[lend-1] == '\n') ||
                (lend > 2 && buff[lend-2] == '\n' && buff[lend-3] == '\r')) {
                buffpos = lend + 1;
                break;
            }

            for (split = lbegin; split < lend && buff[split] != ':'; split++);
            if (split != lend) {
                std::string key (buff + lbegin, split - lbegin);
                std::string val (buff + split + 2, lend - split - 3);
                header[key] = val;
                std::cout << "KV (" + key + ", " + val + ")\n";
            }

            lbegin = lend + 1;
        }
    }
}

int
HttpConnection::read (char *buff, int len)
{
    if (bufflen == -1) {
        return socket->read (buff, len);
    } else if (buffpos < bufflen) {
        int copylen = bufflen - buffpos;
        if (len < copylen) {
            copylen = len;
        }

        for (int i = 0; i < copylen; i++) {
            buff[i] = this->buff[buffpos + i];
        }

        if (len < bufflen - buffpos) {
            buffpos += len;
        } else {
            bufflen = -1;
            len = copylen;
        }

        return len;
    } else {
        bufflen = -1;
        return socket->read (buff, len);
    }

    return 0;
}

int
HttpConnection::get_content_length ()
{
    if (header.count ("Content-Length") > 0) {
        return Utils::parseInt (header["Content-Length"]);
    } else {
        return -1;
    }
}
