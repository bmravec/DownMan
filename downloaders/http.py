#
#       http.py
#
#       Copyright 2010 Brett Mravec <brett.mravec@gmail.com>
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#       MA 02110-1301, USA.

import re
from threading import Thread
import pycurl

from downloaders.download import *

class HttpDownload (Download):
    def __init__ (self, url, downman):
        self.url = url
        self.name = re.search ('([^\/]*)$', self.url).group (1)
        self.downman = downman

    def start_get_info (self, state_cb=None):
        Download.start_get_info (self, state_cb)

        self.tfile = HttpNoBody (self.url)
        self.tfile.completed_cb = self.handle_info_completed
        self.tfile.start ()

    def handle_info_completed (self, tfile):
        self.total = tfile.size

        if tfile.response_code >= 200 and tfile.response_code < 400:
            self.status = 'Online'
            self.set_state (STATE_INFO_COMPLETED)
        else:
            self.status = 'Offline'
            self.set_state (STATE_NOT_FOUND)

    def start_download (self, state_cb=None):
        Download.start_download (self, state_cb)

        filename = re.search ('([^\/]*)$', self.url).group (1)

        self.tfile = WriteFile (self.url, filename)
        self.tfile.completed_cb = self.download_completed
        self.tfile.progress_cb = self.download_progress
        self.tfile.start ()

        self.status = 'Downloading...'
        self.set_state (STATE_DOWNLOADING)

    def download_progress (self, dt, dd, ut, ud):
        self.downloaded = dd
        self.total = dt

    def download_completed (self, wfile):
        self.status = 'Download Complete'
        self.set_state (STATE_COMPLETED)

class HttpNoBody (Thread):
    completed_cb = None

    def __init__ (self, url):
        Thread.__init__ (self)
        self.url = url

    def run (self):
        self.c = pycurl.Curl ()
        self.c.setopt (pycurl.URL, self.url)
        self.c.setopt (pycurl.NOBODY, 1)

        self.c.perform ()

        self.response_code = self.c.getinfo (pycurl.RESPONSE_CODE)
        self.size = self.c.getinfo (pycurl.CONTENT_LENGTH_DOWNLOAD)

        self.c.close ()

        if self.completed_cb != None:
            self.completed_cb (self)

    def write_cb (self, buf):
        self.contents = self.contents + buf

    def close (self):
        self.c.close ()
