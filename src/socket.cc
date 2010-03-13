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

#include <cstdio>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "socket.h"

Socket::Socket (const std::string &host, int port) : connected (false)
{
    sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror ("Error opening socket");
        return;
    }

    struct sockaddr_in serv_addr;
    struct hostent *server;

    server = gethostbyname (host.c_str ());
    bzero ((char*) &serv_addr, sizeof (serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons (port);

    bcopy ((char*) server->h_addr,
           (char*) &serv_addr.sin_addr.s_addr,
           server->h_length);

    if (::connect (sockfd, (const sockaddr*) &serv_addr, sizeof (serv_addr)) < 0) {
        perror ("Error connecting socket");
        return;
    }

    connected = true;
}

Socket::~Socket ()
{
    if (connected) {
        connected = false;
        close (sockfd);
    }
}

int
Socket::write (std::string &str)
{
    return write (str.c_str (), str.size ());
}

int
Socket::write (const char *buff, int len)
{
    return ::write (sockfd, buff, len);
}

int
Socket::read (char *buff, int size)
{
    return ::read (sockfd, buff, size);
}
