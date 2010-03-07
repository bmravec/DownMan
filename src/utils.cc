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
#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>

#include "utils.h"
#include "app-config.h"

std::string
Utils::size_to_string (size_t size)
{
    const char *ss[] = { "Kb", "Mb", "Gb", "Tb" };

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
    return parseInt (str.c_str ());
}

int
Utils::parseInt (const char *str)
{
    int val;
    std::istringstream iss (str);
    iss >> val;
    return val;
}

int
Utils::parseHexInt (const std::string &str)
{
    return parseHexInt (str.c_str ());
}

int
Utils::parseHexInt (const char *str)
{
    int val;
    std::istringstream iss (str);
    iss >> std::hex >> val;
    return val;
}

std::string
Utils::formatInt (int val)
{
    std::stringstream out;
    out << val;
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
Utils::createDownloadFilename (std::string &name)
{
    std::string path = Config::Instance ().get_property (Config::DOWNLOAD_DIRECTORY);

    mkdir (path.c_str (), 0700);
    path += "/" + name;

    return path;
}

std::string
Utils::createDownloadFilename (const char *name)
{
    std::string sname (name);
    return createDownloadFilename (sname);
}

int
Utils::getFileSize (std::string &name)
{
    return getFileSize (name.c_str ());
}

int
Utils::getFileSize (const char *name)
{
    struct stat ostat;
    if (stat (name, &ostat) == 0) {
        return ostat.st_size;
    } else {
        return 0;
    }
}

bool
Utils::removePath (std::string &name)
{
    return removePath (name.c_str ());
}

bool
Utils::removePath (const char *name)
{
    struct stat ostat;

    if (stat (name, &ostat) == 0) {
        if (S_ISDIR (ostat.st_mode)) {
            struct dirent *dp;
            DIR *dir = opendir (name);
            while ((dp = readdir (dir)) != NULL) {
                if (!strcmp (dp->d_name, ".") ||
                    !strcmp (dp->d_name, "..")) {
                    continue;
                }
                std::string lpath = name;
                lpath += "/";
                lpath += dp->d_name;
                removePath (lpath.c_str ());
            }
            closedir (dir);

            std::cout << "Remove Dir: " << name << std::endl;
            remove (name);
        } else {
            std::cout << "Remove File: " << name << std::endl;
            remove (name);
        }

        return true;
    } else {
        return false;
    }
}

std::string
Utils::createConfigFilename (std::string &name)
{
    std::string path (getenv ("HOME"));
    path += "/.config/downman";

    mkdir (path.c_str (), 0700);

    path += "/" + name;

    return path;
}

std::string
Utils::createConfigFilename (const char *name)
{
    std::string sname (name);
    return createConfigFilename (sname);
}

std::string
Utils::getDefaultDownloadDirectory ()
{
    std::string path (getenv ("HOME"));
    path += "/Downloads";

    return path;
}

std::string
Utils::getImageResource (const char *name)
{
    std::string path = SHARE_DIR;

    path += "/imgs/";
    path += name;

    return path;
}
