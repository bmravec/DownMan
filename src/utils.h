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

/**
 * Various utilities that do not fit anywhere else.
 */
class Utils {
    public:
        /**
         * Convert a byte size to a human readable string.
         * Takes a size and converts it to a string expressing a decimal and a
         * SI prefix on bytes like 100.0 MB
         * @param size number of bytes to convert
         * @return human readable string expressing the given byte count
         */
        static std::string size_to_string (size_t size);

        /**
         * Parse an integer from a string.
         * Parse the first integer in the given string
         * @param str string to parse
         * @return integer value or 0 for an error
         */
        static int parseInt (const std::string &str);

        /**
         * Parse an hexadecimal integer from a string.
         * Parse the first hexadecimal integer in the given string
         * @param str string to parse
         * @return integer value or 0 for an error
         */
        static int parseHexInt (const std::string &str);

        /**
         * Format an integer and store in a string
         * @param val integer value to write to a string
         * @return string representation of an integer
         */
        static std::string formatInt (int val);

        /**
         * Format an integer int hexadecimal and store in a string
         * @param val integer value to write to a string
         * @return string representation of an integer
         */
        static std::string formatHexInt (int val);

        /**
         * Format a double and store in a string
         * @param val double value to write to a string
         * @return string representation of a double
         */
        static std::string formatDouble (double val);

        /**
         * Get an absolute path given a name for a download.
         * Given a file name, the full path of a download is returned taking
         * into account the stored download directory from Config.  The path
         * will also be created if it does not already exist.
         * @param name file name to get the path for
         * @return full path to the file including the filename itself
         */
        static std::string createDownloadFilename (const std::string &name);

        /**
         * Get the file size for a file at the path passed in.
         * @param name fullname of the file to lookup
         * @return size of the file or -1 if the file does not exist
         */
        static int getFileSize (const std::string &name);

        /**
         * Delete the path from the hard drive.
         * Removes the file or directory specified by name from the hard drive
         * @param name absolute path to delete
         * @return true if the path was deleted successfully, or false if an
         *         error occured.
         */
        static bool removePath (const std::string &name);

        /**
         * Get an absolute path to config file with the given name.
         * Take the file name passed in and join it with the path to the config
         * directory and return the result
         * @param name name of the file to make the path to
         * @return absolute path to the file
         */
        static std::string createConfigFilename (const std::string &name);

        /**
         * Create the default download directory.
         * This should only be called when creating a new config file as the
         * rest of the time, the default should be loaded from the config file.
         * @return absolute path to the default directory
         */
        static std::string getDefaultDownloadDirectory ();

        /**
         * Return the absolute path to the named resource.
         * @param name resource to provide the path to
         * @return absolute path to the named resource
         */
        static std::string getImageResource (const std::string &name);

        /**
         * Escape the string to be used as a url.
         * @param str string to escape
         * @return escaped string
         */
        static std::string escape (const std::string &str);

        /**
         * Unescape the string.
         * @param url url string to unescape
         * @return unescaped string
         */
        static std::string unescape (const std::string &url);
};

#endif /* __UTILS_H__ */
