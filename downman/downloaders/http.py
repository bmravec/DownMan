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

import re, os, os.path
from threading import Thread
import pycurl

from download import *
from writefile import WriteFile

class HttpDownload (Download):
    pdtotal = -1

    def __init__ (self, url, downman):
        self.url = url
        self.name = re.search ('([^\/]*)$', self.url).group (1)
        self.downman = downman
        self.speed = downman.speedlimit.create_download ()

    def start_get_info (self):
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

    def start_download (self):
        dpath = self.downman.config.get_property ('DefaultDownloadDirectory')

        self.location = os.path.join (dpath, re.search ('([^\/]*)$', self.url).group (1))

        rfrom = None

        if os.path.exists (self.location):
            s = os.stat (self.location)

            if s.st_size == self.total:
                self.set_state (STATE_COMPLETED)
                return
            if s.st_size == self.downloaded:
                rfrom = self.downloaded

        self.tfile = WriteFile (self.url, self.location, rfrom=rfrom)
        self.tfile.completed_cb = self.download_completed
        self.tfile.progress_cb = self.download_progress
        self.tfile.start ()

        self.status = 'Downloading...'
        self.set_state (STATE_DOWNLOADING)

    def download_progress (self, dt, dd, ut, ud):
        self.downloaded = dd
        self.total = dt

        diff = dd - self.pdtotal
        self.pdtotal = dd
        val = self.speed.update_downloaded (diff)
        self.speed.wait (val)

    def download_completed (self, wfile):
        self.status = 'Download Complete'
        self.set_state (STATE_COMPLETED)

    def startup (self, data):
        self.name = data['name']
        self.url = data['url']
        self.downloaded = float (data['downloaded'])
        self.total = float (data['total'])
        self.state = int (data['state'])
        self.location = data['location']

    def shutdown (self):
        data = {}

        if self.state == STATE_DOWNLOADING:
            self.tfile.close ()
            self.state = STATE_QUEUED
        elif self.state == STATE_WAITING:
            self.timeout.cancel ()
            self.state = STATE_QUEUED
        elif self.state == STATE_INFO or self.state == STATE_INFO_COMPLETED:
            self.tfile.close ()
            return

        data['name'] = self.name
        data['url'] = self.url
        data['downloaded'] = str (self.downloaded)
        data['total'] = str (self.total)
        data['state'] = str (self.state)
        data['match'] = 'http'
        data['location'] = self.location

        return data

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
