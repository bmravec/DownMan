/*
 *      dregex.cc
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

#include <iostream>

#include "dregex.h"

DRegex::DRegex (std::string &m) : m_str (m)
{
    regcomp (&pattern, m_str.c_str (), 0);
}

DRegex::DRegex (const char *m)
{
    m_str = m;

    regcomp (&pattern, m_str.c_str (), 0);
}

DRegex::~DRegex ()
{

}

bool
DRegex::match (std::string &str)
{
    size_t no_sub = pattern.re_nsub + 1;
    regmatch_t *result = new regmatch_t[no_sub];

    bool res = regexec (&pattern, str.c_str (), no_sub, result, 0) == 0;

    delete result;

    return res;
}

bool
DRegex::find (std::string &str, std::vector<std::string> &m)
{
    return find (str.c_str (), m);
}

bool
DRegex::find (const char *str, std::vector<std::string> &m)
{
    size_t no_sub = pattern.re_nsub + 1;
    regmatch_t *result = new regmatch_t[no_sub];
    bool res = false;

    std::cout << "NOSUB" << no_sub << std::endl;

    if (regexec (&pattern, str, no_sub, result, 0) == 0) {
        for (int i = 0; i < no_sub; i++) {
            std::string smatch (str + result[i].rm_so, result[i].rm_eo - result[i].rm_so);
            m.push_back (smatch);
        }
        res = true;
    }

    delete result;

    return res;
}

bool
DRegex::find_all (std::string &str, std::vector<std::string> &matches)
{
    return find_all (str.c_str (), matches);
}

bool
DRegex::find_all (const char *str, std::vector<std::string> &matches)
{
    int start = 0;
    size_t no_sub = pattern.re_nsub + 1;
    regmatch_t *result = new regmatch_t[no_sub];
    bool res = false;

    while (regexec (&pattern, str + start, no_sub, result, 0) == 0) {
        std::string smatch (str + start + result->rm_so,
            result->rm_eo - result->rm_so);
        matches.push_back (smatch);
        start += result->rm_eo;
        res = true;
    }

    delete result;

    return res;
}
