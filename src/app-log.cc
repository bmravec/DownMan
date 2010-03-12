/*
 *      app-log.cc
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

#include "app-log.h"
#include "utils.h"

int LogLevel::next_log_number = 0;
const LogLevel Log::LL_NONE ("None");
const LogLevel Log::LL_ERROR ("Error");
const LogLevel Log::LL_INFO ("Info");
const LogLevel Log::LL_DEBUG ("Debug");
const LogLevel Log::LL_ALL ("All");

Log::Log () :
    config (Config::Instance ())
{
    property_changed (Config::LOG_LEVEL, config.get_property (Config::LOG_LEVEL));
    config.get_signal (Config::LOG_LEVEL).connect (sigc::mem_fun (*this, &Log::property_changed));
}

Log::~Log ()
{

}

void
Log::set_log_level (LogLevel &level)
{
    config.set_property (Config::LOG_LEVEL, level.get_name ());
}

void
Log::property_changed (const std::string &key, const std::string &val)
{
    if (key == Config::LOG_LEVEL) {
        if (val == LL_ERROR.get_name ()) {
            log_level = LL_ERROR.get_level ();
        } else if (val == LL_INFO.get_name ()) {
            log_level = LL_INFO.get_level ();
        } else if (val == LL_DEBUG.get_name ()) {
            log_level = LL_DEBUG.get_level ();
        } else if (val == LL_ALL.get_name ()) {
            log_level = LL_ALL.get_level ();
        } else {
            log_level = LL_NONE.get_level ();
        }
    }
}

void
Log::log_error (const std::string &str)
{
    if (log_level >= LL_ERROR.get_level ()) {
        std::cout << "<error> " << str << std::endl;
    }
}

void
Log::log_info (const std::string &str)
{
    if (log_level >= LL_INFO.get_level ()) {
        std::cout << "<info> " << str << std::endl;
    }
}

void
Log::log_debug (const std::string &str)
{
    if (log_level >= LL_DEBUG.get_level ()) {
        std::cout << "<debug> " << str << std::endl;
    }
}
