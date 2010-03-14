/*
 *      app-log.h
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

class Log;
class LogLevel;

#ifndef __LOG_H__
#define __LOG_H__

#include <string>

#include "app-config.h"

class LogLevel {
    public:
        LogLevel (const std::string &name) : name (name), log_num (next_log_number++) { }

        int get_level () const { return log_num; }
        std::string get_name () const { return name; }
    private:
        static int next_log_number;

        int log_num;
        std::string name;
};

#define LOG_ERROR(str) (Log::Instance ().log_error (str))
#define LOG_INFO(str) (Log::Instance ().log_info (str))
#define LOG_DEBUG(str) (Log::Instance ().log_debug (str))

class Log {
    public:
        static Log &Instance () {
            static Log log;
            return log;
        }

        static const LogLevel LL_NONE;
        static const LogLevel LL_ERROR;
        static const LogLevel LL_INFO;
        static const LogLevel LL_DEBUG;
        static const LogLevel LL_ALL;

        void set_log_level (LogLevel &level);

        void log_error (const std::string &str);
        void log_info (const std::string &str);
        void log_debug (const std::string &str);

    private:
        Log ();
        ~Log ();

        int log_level;

        Config &config;

        void property_changed (const std::string &key, const std::string &val);
};

#endif /* __LOG_H__ */
