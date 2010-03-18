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

/**
 * Basic regex wrapper class.
 */
class DRegex {
    public:
        /**
         * Create a basic regex.
         * @param m regex string to compile
         */
        DRegex (const std::string &m);
        ~DRegex ();

        /**
         * Check for occurrence of the match string in str.
         * @param str string to search for the match string in
         * @return true if string is found, else false
         */
        bool match (std::string &str) const;

        /**
         * Find the first occurrence.
         * Search for the first occurrence of the match string in the passed in
         * string and return all groups associated with the match string
         * for that occurrence.
         * @param str string to search for the match string
         * @param m list to fill with strings from the match string groups
         * @return true if match was found in str, else false
         */
        bool find (const std::string &str, std::vector<std::string> &m) const;

        /**
         * Find all occurrences.
         * Search for all the occurrences of the match string in the passed in
         * string and returns the main group for every occurrence of the match
         * string.
         * @param str string to search for the match string in
         * @param matches list to fill with the main group of all occurrences
         * @return true if any occurrences were found, else false
         */
        bool find_all (const std::string &str, std::vector<std::string> &matches) const;

        /**
         * Return the match string
         * @return match string
         */
        std::string get_string () const { return m_str; }

    private:
        std::string m_str;
        regex_t pattern;
};

#endif /* __DREGEX_H__ */
