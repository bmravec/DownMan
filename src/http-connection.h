/*
 *      http-connection.h
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

class HttpConnection;

#ifndef __HTTP_CONNECTION_H__
#define __HTTP_CONNECTION_H__

#include <string>
#include <map>

#include "url.h"
#include "socket.h"

/**
 * Basic pull-style http interface.
 * Provides a wrapper around the http protocol.  After a request, data can then
 * be pulled with the read method to get data from the stream.
 */
class HttpConnection {
    public:
        /**
         * Created un-associated connection.
         */
        HttpConnection ();
        ~HttpConnection ();

        /**
         * Initiate a http get request.
         * Connect to host specified in the url and starts a get request using
         * the path given in the url.  After the call is completed, the
         * connection is ready to have data read from it.  Subsequent calls to
         * any request method will reset the connection and start a new one.
         * @param url resource to connect to and download
         * @param start
         * @param end
         * @return true if the connection was successful, false otherwise
         */
        bool send_get_request (const Url &url, int start=-1, int end=-1);

        /**
         * Initiate a http head request.
         * Connect to host specified in the url and start a head request using
         * the path given in the url.  After the call is completed, all the
         * necessary data is retrieved and available with the get_* methods.
         * Subsiquent calls to any request method will reset the connection and
         * start a new one.
         * @param url resource to connect to and retrieve information about
         * @return true if the connection was successful, false otherwise
         */
        bool send_head_request (const Url &url);

        /**
         * Initiate a http post request.
         * Connect to host specified in the url and start a post request using
         * the path given in the url and the data given in post_data.  The
         * post_data is correctly encoded and send in the body of the post
         * request.  After the call is completed, the connection is ready to
         * have data read from it.  Subsiquent calls to any request method will
         * reset the connection and start a new one.
         * @param url resource to connect to and retrieve information about
         * @param post_data dictionary to be encoded and send as the body of the
         *                  post request
         * @return true if the connection was successful, false otherwise
         */
        bool send_post_request (const Url &url, std::map<std::string, std::string> &post_data);

        /**
         * Read data from the connection
         * Read data from the connection and store it in the buffer passed into
         * the method.  The method will not necessarily fill the buffer.  The
         * argument should be used as a maximum value and multiple reads should
         * be used to get all the data that is required.
         * @param buff buffer to fill with data
         * @param len maximum amount of data to read from the connection
         * @return length of valid data that was written to the buffer, or -1
         *         indicating that the object is not connected to a valid host
         */
        int read (char *buff, int len);

        /**
         * Get the http response code for the current request.
         * @return valid http response code or -1 to indicate that a response
         *         code is not currently available
         */
        int get_response_code ();

        /**
         * Get the human readable response for the current request.
         * @return response or empty string if not connected
         */
        std::string get_code () { return code; }

        /**
         * Get the current content length.
         * Fetches the current content length from the download headers.
         * @return returns content length value or -1 to indicate that the
         *         server did not specify the length of the content
         */
        int get_content_length ();

    private:
        static const std::string USER_AGENT;

        Url *url;
        Socket *socket;

        std::map<std::string, std::string> header;
        std::string version;
        int response_code;
        std::string code;

        char *buff;
        int bufflen;
        int buffpos;

        int clength;
        int cpos;

        void reset_connection ();
        bool read_header ();
        bool read_line (std::string &line);
};

#endif /* __HTTP_CONNECTION_H__ */
