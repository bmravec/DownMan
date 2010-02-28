/*
 *      http-connection.h
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

class HttpConnection;

#ifndef __HTTP_CONNECTION_H__
#define __HTTP_CONNECTION_H__

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "download.h"
#include "url.h"
#include "socket.h"

class HttpConnection {
    public:
        HttpConnection ();
        ~HttpConnection ();

        bool send_get_request (Url &url, int start=-1, int end=-1);
        bool send_head_request (Url &url);
        bool send_post_request (Url &url, const char *post_data);

        int read (char *buff, int len);

        int get_response_code () { return response_code; }
        std::string get_code () { return code; }
        int get_content_length ();

    private:
        Url *url;
        Socket *socket;

        std::map<std::string, std::string> header;
        std::string version;
        int response_code;
        std::string code;

        char *buff;
        int bufflen;
        int buffpos;

        int clength;
        int cpos;

        bool read_header ();
};

#endif /* __HTTP_CONNECTION_H__ */
