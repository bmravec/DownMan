/*
 *      dregex.h
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

class DRegex;

#ifndef __DREGEX_H__
#define __DREGEX_H__

#include <string>
#include <vector>
#include <regex.h>

class DRegex {
    public:
        DRegex (std::string &m);
        DRegex (const char *m);
        ~DRegex ();

        bool match (std::string &str);
        bool find_all (std::string &str, std::vector<std::string> &matches);
        bool find_all (const char *str, std::vector<std::string> &matches);

    private:
        std::string m_str;
        regex_t pattern;
};

#endif /* __DREGEX_H__ */
