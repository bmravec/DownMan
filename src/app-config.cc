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

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "app-config.h"
#include "utils.h"

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
    set_property (DOWNLOAD_DIRECTORY, Utils::getDefaultDownloadDirectory ());
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

    std::string configpath = Utils::createConfigFilename ("config.xml");

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
    int rc;
    xmlTextWriterPtr writer;
    xmlChar *tmp;

    std::string configpath = Utils::createConfigFilename ("config.xml");

    writer = xmlNewTextWriterFilename (configpath.c_str (), 0);
    if (writer == NULL) {
        std::cerr << "Error creating xml writer\n";
        return;
    }

    xmlTextWriterStartDocument (writer, NULL, "utf-8", NULL);
    xmlTextWriterStartElement (writer, BAD_CAST "downman");

    { // Write config block
        std::map<std::string, PropertySet>::iterator iter;
        xmlTextWriterStartElement (writer, BAD_CAST "config");

        for (iter = properties.begin (); iter != properties.end (); iter++) {
            xmlTextWriterWriteElement (writer,
                BAD_CAST iter->first.c_str (), BAD_CAST iter->second.val.c_str ());
        }

        xmlTextWriterEndElement (writer);
    }

    { // Write download blocks
        std::vector<std::map<std::string, std::string> >::iterator viter;
        std::map<std::string, std::string>::iterator miter;

        for (viter = dlist.begin (); viter != dlist.end (); viter++) {
            xmlTextWriterStartElement (writer, BAD_CAST "download");
            for (miter = viter->begin (); miter != viter->end (); miter++) {
                xmlTextWriterWriteElement (writer,
                    BAD_CAST miter->first.c_str (), BAD_CAST miter->second.c_str ());
            }

            xmlTextWriterEndElement (writer);
        }
    }

    xmlTextWriterEndElement (writer);
    xmlTextWriterEndDocument (writer);

    xmlFreeTextWriter (writer);
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
