/*
 *      ftp-connection.h
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

class FtpConnection;

#ifndef __FTP_CONNECTION_H__
#define __FTP_CONNECTION_H__

#include <iostream>
#include <string>

#include "socket.h"

class FtpConnection {
    public:
        FtpConnection (std::string &host, int port=21);
        ~FtpConnection ();

        bool send_get_request (std::string path, int start=-1);
        bool get_file_size (std::string path, int *size);

        int read (char *buff, int len);

    private:
        std::string host;
        int port;
        std::string path;

        Socket *csock;
        Socket *dsock;

        char *buff;
        int bufflen;
        int buffpos;
};

#endif /* __FTP_CONNECTION_H__ */
