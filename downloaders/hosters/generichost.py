#
#       generichost.py
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

from downloaders.download import *

from downloaders.tempfile import TempFile
from downloaders.writefile import WriteFile
from downloaders.timeout import Timeout

class GenericHost (Download):
    proto = 'http'
    case_handlers = []

    def __init__ (self, url, downman):
        self.url = url
        self.name = url
        self.downman = downman

    def start_get_info (self, state_cb=None):
        Download.start_get_info (self, state_cb)

        self.tfile = TempFile (self.url)
        self.tfile.completed_cb = self.stage_download_completed
        self.tfile.start ()

        self.set_state (STATE_INFO)

    def start_download (self, state_cb=None):
        Download.start_download (self, state_cb)

        self.tfile = TempFile (self.url)
        self.tfile.completed_cb = self.stage_download_completed
        self.tfile.start ()

        self.set_state (STATE_CONNECTING)

    def stage_download_completed (self, tfile):
        for handler in self.case_handlers:
            m = re.search (handler[0], self.tfile.contents)
            if m != None:
                handler[1] (m)

    def download_completed (self, wfile):
        self.status = 'Download Complete'
        self.set_state (STATE_COMPLETED)

    def download_progress (self, dt, dd, ut, ud):
        self.downloaded = dd
        if dt != 0:
            self.total = dt

    def print_progress (self, num):
        self.status = 'Starting in %d seconds' % (num)
        self.set_state (STATE_WAITING)

    def close (self):
        if self.tfile:
            self.tfile.close ()
