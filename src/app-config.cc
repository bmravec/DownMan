/*
 *      config.cc
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
#include <cstring>
#include <cstdlib>

#include "app-config.h"

#define DEFAULT_DOWNLOAD_DIRECTORY "~/Downloads"

typedef enum {
    STATE_NULL = 0,
    STATE_DOWNMAN,
    STATE_CONFIG,
    STATE_CONFIG_PROPERTY,
    STATE_DOWNLOAD,
    STATE_DOWNLOAD_PROPERTY,
    STATE_ERROR,
} SaxParserState;

typedef struct {
    Config *config;
    SaxParserState state;
    std::string key, val;
    std::map<std::string, std::string> tdownload;
} SaxParserData;

const std::string Config::MAX_DOWNLOAD_SPEED ("MaxDownloadSpeed");
const std::string Config::MAX_UPLOAD_SPEED ("MaxUploadSpeed");
const std::string Config::MAX_NUM_DOWNLOADS ("MaxNumDownloads");
const std::string Config::DOWNLOAD_DIRECTORY ("DownloadDirectory");

Config::Config () : downloads (new std::vector<std::map<std::string, std::string> >)
{
    set_property (DOWNLOAD_DIRECTORY, DEFAULT_DOWNLOAD_DIRECTORY);
    set_property (MAX_UPLOAD_SPEED, "-1");
    set_property (MAX_DOWNLOAD_SPEED, "-1");
    set_property (MAX_NUM_DOWNLOADS, "1");
}

Config::~Config ()
{

}

sigc::signal<void, const std::string&, const std::string&>&
Config::get_signal (const std::string &key)
{
    return properties[key].sig;
}

std::string
Config::get_property (const std::string &key)
{
    return properties[key].val;
}

void
Config::set_property (const std::string &key, const std::string &val)
{
    properties[key].val = val;

    properties[key].sig (key, val);
}

void
Config::set_property (const std::string &key, const char *val)
{
    std::string sval (val);

    set_property (key, sval);
}

bool
Config::load_settings ()
{
    xmlSAXHandlerPtr shandle = new xmlSAXHandler;
    memset (shandle, 0, sizeof (xmlSAXHandler));

    shandle->startElement = Config::ac_start_element;
    shandle->endElement = Config::ac_end_element;
    shandle->characters = Config::ac_characters;

    SaxParserData *data = new SaxParserData;
    data->config = this;
    data->state = STATE_NULL;

    char *home = getenv ("HOME");
    std::string configpath (home);
    configpath += "/.config/downman/config.xml";

    xmlSAXUserParseFile (shandle, data, configpath.c_str ());

    delete data;
    delete shandle;
}

std::vector<std::map<std::string, std::string> >*
Config::load_downloads ()
{
    if (downloads) {
        std::vector<std::map<std::string, std::string> > *d = downloads;
        downloads = NULL;
        return d;
    } else {
        return NULL;
    }
}

void
Config::save (std::vector<std::map<std::string, std::string> > &dlist)
{
    std::cout << "Config::save (&dlist): stub\n";
}

void
Config::ac_start_element (void *user_data, const xmlChar *name, const xmlChar **attrs)
{
    SaxParserData *sdata = (SaxParserData*) user_data;

    std::string sname ((const char*) name);

    switch (sdata->state) {
        case STATE_NULL:
            if (sname == "downman") {
                sdata->state = STATE_DOWNMAN;
            }

            break;
        case STATE_DOWNMAN:
            if (sname == "config") {
                sdata->state = STATE_CONFIG;
            } else if (sname == "Download") {
                sdata->state = STATE_DOWNLOAD;
                sdata->tdownload.clear ();
            }

            break;
        case STATE_CONFIG:
            sdata->key = sname;
            sdata->state = STATE_CONFIG_PROPERTY;
            break;
        case STATE_DOWNLOAD:
            sdata->key = sname;
            sdata->state = STATE_DOWNLOAD_PROPERTY;
            break;
    };
}

void
Config::ac_end_element (void *user_data, const xmlChar *name)
{
    SaxParserData *sdata = (SaxParserData*) user_data;

    std::string sname ((const char*) name);

    switch (sdata->state) {
        case STATE_DOWNMAN:
            sdata->state = STATE_NULL;
            break;
        case STATE_CONFIG:
            sdata->state = STATE_DOWNMAN;
            break;
        case STATE_CONFIG_PROPERTY:
            sdata->state = STATE_CONFIG;
            if (sdata->key == sname) {
                sdata->config->set_property (sdata->key, sdata->val);
            } else {
                sdata->state = STATE_ERROR;
            }

            break;
        case STATE_DOWNLOAD:
            sdata->state = STATE_DOWNMAN;
            sdata->config->downloads->push_back (sdata->tdownload);
            break;
        case STATE_DOWNLOAD_PROPERTY:
            sdata->state = STATE_DOWNLOAD;
            sdata->tdownload[sdata->key] = sdata->val;
            break;
    };
}

void
Config::ac_characters (void *user_data, const xmlChar *ch, int len)
{
    SaxParserData *sdata = (SaxParserData*) user_data;

    std::string str ((const char*) ch, len);

    if (sdata->state == STATE_DOWNLOAD_PROPERTY ||
        sdata->state == STATE_CONFIG_PROPERTY) {
        sdata->val = str;
    }
}
