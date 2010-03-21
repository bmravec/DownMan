/*
 *      utils.cc
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
#include <dirent.h>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>

#include "app-log.h"
#include "utils.h"
#include "app-config.h"

std::string
Utils::size_to_string (size_t size)
{
    const char *ss[] = { "KB", "MB", "GB", "TB" };

    char digits = 2;
    char str[50];
    double num = size;

    if (size < 1024) {
        sprintf (str, "%d B", size);
        return std::string (str);
    }

    for (int i = 0; i < 3; i++) {
        num /= 1024.0;

        if (num < 1024) {
            if (num > 100) {
                digits = 1;
            }

            sprintf (str, "%.*f %s", digits, num, ss[i]);
            return std::string (str);
        }
    }

    if (num > 1000) {
        digits = 0;
    } else if (num > 100) {
        digits = 1;
    }

    sprintf (str, "%.*f %s", digits, num, ss[3]);
    return std::string (str);
}

int
Utils::parseInt (const std::string &str)
{
    if (str[0] >= '0' && str[0] <= '9' || str[0] == '-') {
        int val;
        std::istringstream iss (str);
        iss >> val;
        return val;
    } else {
        return 0;
    }
}

int
Utils::parseHexInt (const std::string &str)
{
    if ((str[0] >= '0' && str[0] <= '9') ||
        (str[0] >= 'a' && str[0] <= 'f') ||
        (str[0] >= 'A' && str[0] <= 'F')) {
        int val;
        std::istringstream iss (str);
        iss >> std::hex >> val;
        return val;
    } else {
        return 0;
    }
}

std::string
Utils::formatInt (int val)
{
    std::stringstream out;
    out << val;
    return out.str ();
}

std::string
Utils::formatHexInt (int val)
{
    std::stringstream out;
    out << std::hex << val;
    return out.str ();
}

std::string
Utils::formatDouble (double val)
{
    std::stringstream out;
    out << val;
    return out.str ();
}

std::string
Utils::createDownloadFilename (const std::string &name)
{
    std::string path = Config::Instance ().get_property (Config::DOWNLOAD_DIRECTORY);

    mkdir (path.c_str (), 0700);
    path += "/" + name;

    return path;
}

int
Utils::getFileSize (const std::string &name)
{
    struct stat ostat;
    if (stat (name.c_str (), &ostat) == 0) {
        return ostat.st_size;
    } else {
        return -1;
    }
}

bool
Utils::removePath (const std::string &name)
{
    struct stat ostat;

    if (stat (name.c_str (), &ostat) == 0) {
        if (S_ISDIR (ostat.st_mode)) {
            struct dirent *dp;
            DIR *dir = opendir (name.c_str ());
            while ((dp = readdir (dir)) != NULL) {
                if (!strcmp (dp->d_name, ".") ||
                    !strcmp (dp->d_name, "..")) {
                    continue;
                }

                removePath (name + "/" + dp->d_name);
            }
            closedir (dir);

            LOG_INFO ("Remove Dir: " + name);
            remove (name.c_str ());
        } else {
            LOG_INFO ("Remove File: " + name);
            remove (name.c_str ());
        }

        return true;
    } else {
        return false;
    }
}

std::string
Utils::createConfigFilename (const std::string &name)
{
    std::string path (getenv ("HOME"));
    path += "/.config/downman";

    mkdir (path.c_str (), 0700);

    path += "/" + name;

    return path;
}

std::string
Utils::getDefaultDownloadDirectory ()
{
    std::string path (getenv ("HOME"));
    path += "/Downloads";

    return path;
}

std::string
Utils::getImageResource (const std::string &name)
{
    std::string path = SHARE_DIR;

    path += "/imgs/";
    path += name;

    return path;
}

std::string
Utils::escape (const std::string &str)
{
    std::string ret;

    for (int i = 0; i < str.size (); i++) {
        if ((str[i] >= '0' && str[i] <= '9') ||
            (str[i] >= 'A' && str[i] <= 'Z') ||
            (str[i] >= 'a' && str[i] <= 'z')) {
            ret += str[i];
        } else {
            if (str[i] & 0xff < 16) {
                ret += "%0" + Utils::formatHexInt (str[i] & 0xff);
            } else {
                ret += "%" + Utils::formatHexInt (str[i] & 0xff);
            }
        }
    }

    return ret;
}

static int
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
Utils::unescape (const std::string &url)
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
