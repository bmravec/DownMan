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

#include "utils.h"

std::string
size_to_string (size_t size)
{
    const char *ss[] = { "KB", "MB", "GB", "TB" };

    char digits = 2;
    char str[50];
    double num = size;

    if (size < 1024) {
        sprintf (str, "%d Bytes", size);
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
