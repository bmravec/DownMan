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

#include "../config.h"

#include <cstring>

#include "app-log.h"
#include "http-connection.h"
#include "utils.h"

#define BUFFER_TOTAL 10240

const std::string HttpConnection::USER_AGENT (PACKAGE_NAME "/" PACKAGE_VERSION);

HttpConnection::HttpConnection (const std::string &h, int p) :
    socket (NULL), buff (new char [BUFFER_TOTAL]), bufflen (0), buffpos (0),
    cpos (0), clength (-1), response_code (-1), host (h), port (p != 0 ? p : 80)
{

}

HttpConnection::~HttpConnection ()
{
    reset_connection ();

    delete buff;
}

void
HttpConnection::reset_connection ()
{
    if (socket != NULL) {
        delete socket;
        socket = NULL;

        bufflen = 0;
        buffpos = 0;

        response_code = -1;
        code = "";
    }
}

bool
HttpConnection::send_get_request (const std::string &path, int start, int end)
{
    reset_connection ();

    socket = new Socket (host, port);

    if (socket->is_connected ()) {
        std::string request = "GET " + path + " HTTP/1.1\r\n";
        request += "Host: " + host + "\r\n";
        request += "User-Agent: " + USER_AGENT +"\r\n";

        if (start != -1) {
            request += "Range: bytes=";
            request += Utils::formatInt (start);
            request += "-";
            if (end != -1) {
                request += Utils::formatInt (end);
            }
            request += "\r\n";
        }

        LOG_DEBUG ("Http Get Request:\n" + request);

        request += "\r\n";

        socket->write (request);

        return read_header ();
    }

    return false;
}

bool
HttpConnection::send_head_request (const std::string &path)
{
    reset_connection ();

    socket = new Socket (host, port);

    if (socket->is_connected ()) {
        std::string request = "HEAD " + path + " HTTP/1.1\r\n";
        request += "Host: " + host + "\r\n";
        request += "User-Agent: " + USER_AGENT +"\r\n";

        LOG_DEBUG ("Http Head Request:\n" + request);

        request += "\r\n";

        socket->write (request);

        return read_header ();
    }

    return false;
}

bool
HttpConnection::send_post_request (const std::string &path, std::map<std::string, std::string> &post_data)
{
    reset_connection ();

    socket = new Socket (host, port);

    if (socket->is_connected ()) {
        std::string request = "POST " + path + " HTTP/1.1\r\n";
        request += "Host: " + host + "\r\n";
        request += "User-Agent: " + USER_AGENT +"\r\n";
        request += "Content-Type: application/x-www-form-urlencoded\r\n";

        std::string post_str;

        std::map<std::string, std::string>::iterator iter;
        for (iter = post_data.begin (); iter != post_data.end (); iter++) {
            post_str += (*iter).first + "="
                      + Utils::escape ((*iter).second) + "&";
        }

        if (post_str.size () > 0) {
            post_str.erase (post_str.size () - 1);
            request += "Content-Length: " + Utils::formatInt (post_str.size ()) + "\r\n";
        } else {
            LOG_DEBUG ("Invalid post request: " + path);
            return false;
        }

        LOG_DEBUG ("Http Post Request:\n" + request);
        LOG_DEBUG ("Http Post Data:\n" + post_str);

        request += "\r\n";

        socket->write (request);
        socket->write (post_str);

        return read_header ();
    }

    return false;
}

bool
HttpConnection::read_header ()
{
    size_t lbegin, lend, split;

    bufflen = socket->read (buff, BUFFER_TOTAL);
    buffpos = 0;

    header.clear ();

    std::string ldata;

    version = std::string (buff, 8);
    response_code = Utils::parseInt (buff + 9);
    for (lend = 0; !(buff[lend] == '\n' || (lend != 0 && buff[lend - 1] == '\n' && buff[lend] == '\r')); lend++);
    code = std::string (buff + 13, lend - 14);

    LOG_DEBUG ("Http Response Version: " + version);
    LOG_DEBUG ("Http Response Code: " + Utils::formatInt (response_code) + ", " + code);

    std::string hstr ("Http Response Headers:\n");
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
                hstr += "  " + key + ": " + val + "\n";
            }

            lbegin = lend + 1;
        }
    }
    LOG_DEBUG (hstr);

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

    return true;
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
HttpConnection::get_response_code ()
{
    if (socket) {
        return response_code;
    } else {
        return -1;
    }
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
