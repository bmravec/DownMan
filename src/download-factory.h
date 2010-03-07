/*
 *      download-factory.h
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

class DownloadFactory;

#ifndef __DOWNLOAD_FACTORY_H__
#define __DOWNLOAD_FACTORY_H__

#include "download.h"
#include "url.h"

/**
 * Factory to create generic Download objects.
 * Factory class that creates Download objects and returns generic Download
 * objects so the rest of the application can work with the common interface
 * and not have to worry about the specifics of a Download instance.
 */
class DownloadFactory {
    public:
        DownloadFactory ();
        ~DownloadFactory ();

        /**
         * Create new a download from a url.
         * Takes the url object and uses it to create a new download of a
         * specific type.  It then returns a generic download object to be used
         * by the rest of the program.
         * @param url url to be used to create the specific download
         * @return Generic Download object representing the given url or NULL if
         *         the url does not describe a downloadable type of DownMan
         */
        Download *create_download (Url &url);

        /**
         * Build a download from saved data.
         * Takes a dictionary of data describing a saved download and creates
         * the object in the saved state.
         * @param data dictionary using Download::KEY_* as keys describing the
         *             internal state of download to be loaded.
         * @return Generic Download object loading using the input data or NULL
                   if the data did not describe a valid Download object.
         */
        Download *build_download (std::map<std::string, std::string> &data);

    private:
};

#endif /* __DOWNLOAD_FACTORY_H__ */
