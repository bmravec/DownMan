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

#include <libxml/parser.h>
#include <sigc++/sigc++.h>

/**
 * Configuration manager.
 * Configuration manager that loads and stores settings, and also can notify
 * listeners about changes to properties so that they can react to changes.
 */
class Config {
    public:
        static Config &Instance () {
            static Config config;
            return config;
        }

        /**
         * Get the changed signal associated with a given key.
         * Retrieve the property_changed signal for a given configuration key.
         * If the key does not exist, it will be created and a valid signal
         * object will be returned
         * @param key key to lookup the signal for
         * @return valid signal for the given key
         */
        sigc::signal<void, const std::string&, const std::string&> &get_signal (const std::string &key);

        /**
         * Set the value for a specific key.
         * Change the value of a specific key to the value passed in to the
         * function.  Method also sends the associated property_changed signal
         * allowing all the listeners to be notified of the change.  The caller
         * of the method should use the signal to update rather than what is
         * passed into the function.
         * @param key key to store value to
         * @param val value to store
         */
        void set_property (const std::string &key, const std::string &val);

        /**
         * Get value associated with a key.
         * @param key to lookup
         * @return always returns a valid string, will be zero-length if the key
         *         was not already present.
         */
        std::string get_property (const std::string &key);

        /**
         * Load configuration settings from the config.xml file.
         */
        void load_settings ();

        /**
         * Get list of saved downloads.
         * The download list is created during the load_settings call and stored
         * until the load_downloads call is made.  The list is then owned by the
         * caller and should be deleted when the caller is done with it.
         * @return a valid vector containing all saved downloads, an empty
         *         vector if there are no saved downloads, and a NULL pointer if
         *         the function had already been called before.
         */
        std::vector<std::map<std::string, std::string> > *load_downloads ();

        /**
         * Save settings and downloads to config.xml.
         * Save all of the settings and the downloads passed into the function
         * into the save config.xml file.
         * @param dlist list of downloads stored as dictionaries to save
         */
        void save (std::vector<std::map<std::string, std::string> > &dlist);

        static const std::string MAX_DOWNLOAD_SPEED;
        static const std::string MAX_UPLOAD_SPEED;
        static const std::string MAX_NUM_DOWNLOADS;
        static const std::string DOWNLOAD_DIRECTORY;
        static const std::string LOG_LEVEL;

    private:
        Config ();
        ~Config ();

        typedef struct {
            std::string val;
            sigc::signal<void, const std::string&, const std::string&> sig;
        } PropertySet;

        static void ac_start_element (void *user_data, const xmlChar *name, const xmlChar **attrs);
        static void ac_end_element (void *user_data, const xmlChar *name);
        static void ac_characters (void *user_data, const xmlChar *ch, int len);

        std::map<std::string, PropertySet> properties;
        std::vector<std::map<std::string, std::string> > *downloads;
};

#endif /* __CONFIG_H__ */
