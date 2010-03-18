/*
 *      ftp-connection.cc
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

#include "app-log.h"
#include "ftp-connection.h"
#include "utils.h"
#include "dregex.h"

#define BUFFER_TOTAL 51200

FtpConnection::FtpConnection (const std::string &host, int port) :
    host (host), csock (NULL), dsock (NULL),
    buff (new char [BUFFER_TOTAL]), bufflen (0)
{
    if (port == 0) {
        port = 21;
    }

    csock = new Socket (host, port);

    if (csock->is_connected () == false) {
        delete csock;
        csock = NULL;
        return;
    }

    if ((bufflen = csock->read (buff, BUFFER_TOTAL)) <= 0) {
        delete csock;
        csock = NULL;
        return;
    }

    std::string str (buff, bufflen);
    int code = Utils::parseInt (str);

    if (code != 220) {
        delete csock;
        csock = NULL;
        return;
    }

    csock->write ("USER anonymous\n", 15);

    if ((bufflen = csock->read (buff, BUFFER_TOTAL)) > 0) {
//        std::cout << "Result of USER: " << buff;
    }

    csock->write ("PASS downman@example.com\n", 25);

    if ((bufflen = csock->read (buff, BUFFER_TOTAL)) > 0) {
//        std::cout << "Result of USER: " << buff;
    }

    csock->write ("PASV\n", 5);

    if ((bufflen = csock->read (buff, BUFFER_TOTAL)) > 0) {
//        std::cout << "Result of PASV: " << buff;
    }

    DRegex reg ("\\(([0-9]*),([0-9]*),([0-9]*),([0-9]*),([0-9]*),([0-9]*)\\)");
    std::vector<std::string> nums;
    std::string str2 (buff);

    if (reg.find (str2, nums)) {
        std::string newhost = nums[1] + "." + nums[2] + "." + nums[3] + "." + nums[4];
        int dport = Utils::parseInt (nums[5]) * 256 + Utils::parseInt (nums[6]);

        dsock = new Socket (newhost, dport);

        if (dsock->is_connected () == false) {
            LOG_ERROR ("Ftp Connection: Can not connect to data socket");

            delete dsock;
            dsock = NULL;
            delete csock;
            csock = NULL;
            return;
        }
    }
}

FtpConnection::~FtpConnection ()
{
    delete buff;

    if (dsock) {
        std::string cmd ("ABOR\n");
        csock->write (cmd);

        delete dsock;
        dsock = NULL;
    }

    if (csock) {
        std::string cmd ("QUIT\n");
        csock->write (cmd);

        delete csock;
        csock = NULL;
    }
}

bool
FtpConnection::get_file_size (std::string path, int *size)
{
    if (csock == NULL || dsock == NULL) {
        return false;
    }

    std::string cmd = "SIZE " + path + "\n";

    csock->write (cmd);

    bool res = false;

    if ((bufflen = csock->read (buff, 512)) > 0) {
        if (Utils::parseInt (buff) == 213) {
            *size = Utils::parseInt (buff + 4);
            res = true;
        }
    }

    return res;
}

bool
FtpConnection::send_get_request (std::string path, int start)
{
    if (csock == NULL || dsock == NULL) {
        return false;
    }

    std::string tcmd = "TYPE I\n";
    csock->write (tcmd);
    if ((bufflen = csock->read (buff, BUFFER_TOTAL)) > 0) {
//        std::cout << "Result of TYPE: " << buff;
    }

    if (start != -1) {
        std::string cmd = "REST " + Utils::formatInt (start) + "\n";
        csock->write (cmd);

        if ((bufflen = csock->read (buff, BUFFER_TOTAL)) > 0) {
//            std::cout << "Result of REST: " << buff;
        }
    }

    std::string cmd = "RETR " + path + "\n";
    bool res = false;

    csock->write (cmd);

    if ((bufflen = csock->read (buff, BUFFER_TOTAL)) > 0) {
        if (Utils::parseInt (buff) == 150) {
            res = true;
        }
    }

    return res;
}

int
FtpConnection::read (char *buff, int len)
{
    if (csock == NULL || dsock == NULL) {
        return -1;
    }

    return dsock->read (buff, len);
}
