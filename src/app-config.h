/*
 *      config.h
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

class Config;

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <map>
#include <string>
#include <vector>

#include <sigc++/sigc++.h>
#include <libxml/parser.h>

typedef struct {
    std::string val;
    sigc::signal<void, const std::string&, const std::string&> sig;
} PropertySet;

class Config {
    public:
        static Config &Instance () {
            static Config config;
            return config;
        }

        sigc::signal<void, const std::string&, const std::string&> &get_signal (const std::string &key);

        std::string get_property (std::string &key);
        void set_property (const std::string &key, const std::string &val);
        void set_property (const std::string &key, const char *val);

        bool load_settings ();
        std::vector<std::map<std::string, std::string> > *load_downloads ();
        void save (std::vector<std::map<std::string, std::string> > &dlist);

        static const std::string MAX_DOWNLOAD_SPEED;
        static const std::string MAX_UPLOAD_SPEED;
        static const std::string MAX_NUM_DOWNLOADS;
        static const std::string DOWNLOAD_DIRECTORY;

    private:
        Config ();
        ~Config ();

        static void ac_start_element (void *user_data, const xmlChar *name, const xmlChar **attrs);
        static void ac_end_element (void *user_data, const xmlChar *name);
        static void ac_characters (void *user_data, const xmlChar *ch, int len);

        std::map<std::string, PropertySet> properties;
        std::vector<std::map<std::string, std::string> > *downloads;
};

#endif /* __CONFIG_H__ */
