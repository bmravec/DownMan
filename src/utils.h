/*
 *      utils.h
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

#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

class Utils {
    public:
        static std::string size_to_string (size_t size);

        static int parseInt (const std::string &str);
        static int parseInt (const char *str);
        static int parseHexInt (const std::string &str);
        static int parseHexInt (const char *str);

        static std::string formatInt (int val);
        static std::string formatDouble (double val);

        static std::string createDownloadFilename (std::string &name);
        static std::string createDownloadFilename (const char *name);
        static int getFileSize (std::string &name);
        static int getFileSize (const char *name);

        static bool removePath (std::string &name);
        static bool removePath (const char *name);

        static std::string createConfigFilename (std::string &name);
        static std::string createConfigFilename (const char *name);
        static std::string getDefaultDownloadDirectory ();
};

#endif /* __UTILS_H__ */
