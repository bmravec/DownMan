d#
#       writefile.py
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

from threading import Thread
import pycurl
import re

class WriteFile (Thread):
    completed_cb = None
    progress_cb = None

    def __init__ (self, url, filename=None, post_data=None, referer=None):
        Thread.__init__ (self)
        self.url = url
        if filename != None:
            self.filename = filename
        else:
            self.filename = re.search ('([^\/]*)$', url).group (1)

        self.post_data = post_data
        self.referer = referer

    def run (self):
        self.c = pycurl.Curl ()
        self.c.setopt (pycurl.URL, self.url)

        f = open (self.filename, 'wb')
        self.c.setopt (pycurl.WRITEFUNCTION, f.write)

        if self.post_data != None:
            self.c.setopt (pycurl.POSTFIELDS, self.post_data)
            self.c.setopt (pycurl.POST, 1)

        if self.referer != None:
            self.c.setopt (pycurl.REFERER, self.referer)

        if self.progress_cb != None:
            self.c.setopt (pycurl.NOPROGRESS, 0)
            self.c.setopt (pycurl.PROGRESSFUNCTION, self.progress_cb)

        self.c.perform ()
        self.c.close ()
        f.close ()

        if self.completed_cb != None:
            self.completed_cb (self)

    def close (self):
        self.c.close ()
