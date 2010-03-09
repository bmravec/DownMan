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
#include <cstring>

#include "http-connection.h"
#include "utils.h"

#define BUFFER_TOTAL 51200

HttpConnection::HttpConnection () :
    url (NULL), socket (NULL), buff (new char [BUFFER_TOTAL]), bufflen (0), buffpos (0),
    cpos (0), clength (-1)
{

}

HttpConnection::~HttpConnection ()
{
    delete buff;
}

bool
HttpConnection::send_get_request (Url &url, int start, int end)
{
    if (socket == NULL) {
        socket = new Socket (url.get_host ().c_str (), url.get_port () != 0 ? url.get_port () : 80);

        std::string request = "GET " + url.get_path () + " HTTP/1.1\r\n";
        request += "Host: " + url.get_host () + "\r\n";
        request += "User-Agent: DownMan-BMT/1.0\r\n";

        if (start != -1) {
            request += "Range: bytes=";
            request += Utils::formatInt (start);
            request += "-";
            if (end != -1) {
                request += Utils::formatInt (end);
            }
            request += "\r\n";
        }

        request += "\r\n";

        std::cout << "Request: " << request;

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

        std::string request = "HEAD " + url.get_path () + " HTTP/1.1\r\n";
        request += "Host: " + url.get_host () + "\r\n";
        request += "User-Agent: DownMan-BMT/1.0\r\n";
        request += "\r\n";

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

    std::string KEY_TRANSFER_ENCODING ("Transfer-Encoding");
    if (header[KEY_TRANSFER_ENCODING] == "chunked") {
        cpos = 0;

        std::string line;
        if (read_line (line)) {
            clength = Utils::parseHexInt (line);
        }
    } else {
        clength = -1;
    }
}

int
HttpConnection::read (char *retbuff, int len)
{
    if (clength == 0) { // clength == 0 signals end of clustered transfer
        len = 0;
    } else if (clength > 0) {
        // Make len min of len, buff left, and chunk left
        len = len < (bufflen - buffpos) ? len : bufflen - buffpos;
        len = len < (clength - cpos) ? len : clength - cpos;

        memcpy (retbuff, buff + buffpos, len);
        buffpos += len;
        cpos += len;

        // Finished buff
        if (buffpos >= bufflen) {
            bufflen = socket->read (buff, BUFFER_TOTAL);
            buffpos = 0;
        }

        // Finished chunk
        if (cpos >= clength) {
            std::string line;
            read_line (line);
            if (read_line (line)) {
                clength = Utils::parseHexInt (line);
                cpos = 0;
            }
        }
    } else if (bufflen == -1) {
        len = socket->read (retbuff, len);
    } else {
        if (buffpos >= bufflen) {
            delete buff;
            buff = NULL;
            bufflen = -1;
            buffpos = 0;

            len = socket->read (retbuff, len);
        } else {
            len = len < (bufflen - buffpos) ? len : bufflen - buffpos;

            memcpy (retbuff, buff + buffpos, len);
            buffpos += len;
        }
    }

    return len;
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

bool
HttpConnection::read_line (std::string &line)
{
    line = "";
    bool gotr = false;

    for (;true; buffpos++) {
        if (buffpos >= bufflen) {
            bufflen = socket->read (buff, BUFFER_TOTAL);
            buffpos = 0;

            if (bufflen <= 0) {
                return false;
            }
        }

        if (buff[buffpos] == '\r') {
            gotr = true;
        } else if (buff[buffpos] == '\n') {
            buffpos++;
            break;
        } else {
            if (gotr) {
                line += '\r';
                gotr = false;
            }
            line += buff[buffpos];
        }
    }

    return true;
}
