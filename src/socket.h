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

class Socket;

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>

/**
 * Basic wrapper around a c socket.
 */
class Socket {
    public:
        /**
         * Create a socket and connect it.
         */
        Socket (const std::string &host, int port);
        ~Socket ();

        /**
         * Write string out.
         * @param str string to write out
         * @return size of data written out
         */
        int write (std::string &str);

        /**
         * Write character buffer out.
         * @param buff data to write out
         * @param len length of data in the buffer ready to be written
         * @return size of data written out
         */
        int write (const char *buff, int len);

        /**
         * Read data into buffer.
         * @param buff buffer to read data into
         * @param size max size of buffer
         * @return length of data read in or -1 to indicate read error
         */
        int read (char *buff, int size);

        /**
         * Check if the socket is connected
         * @return true if socket is connected, else false
         */
        bool is_connected () { return connected; }

    private:
        int sockfd;
        bool connected;
};

#endif /* __SOCKET_H__ */
