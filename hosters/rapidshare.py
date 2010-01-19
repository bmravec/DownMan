#
#       rapidshare.py
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

from tempfile import TempFile
from writefile import WriteFile
from timeout import Timeout
from generichost import *

class Rapidshare (GenericHost):
    def __init__ (self, url, dm):
        GenericHost.__init__ (self, url, dm)

        self.case_handlers = [
            ('<form id="ff" action="([^"]*)" method="post">', self.handle_start_page),
            ('You have reached the download limit for free\-users', self.handle_download_limit),
            ('Your IP address (\w+\.\w+\.\w+\.\w+) is already downloading a file', self.handle_already_downloading),
            ('we have no more download slots', self.handle_no_slots),
            ('Please try again in (\w+) minutes', self.handle_no_slots_count),
            ('var c=(\d+)', self.handle_download),
        ]

    def handle_start_page (self, match):
        self.aurl = match.group (1)

        m = re.search ('<p class="downloadlink">[^<]*?<font [^>]*?>\| (\d+) (KB|MB|GB)</font></p>', self.tfile.contents)

        self.total = int (m.group (1))
        if m.group (2) == 'KB':
            self.total = self.total * 1024
        if m.group (2) == 'MB':
            self.total = self.total * 1024 * 1024
        if m.group (2) == 'GB':
            self.total = self.total * 1024 * 1024 * 1024

        self.name = re.search ('([^\/]*)$', self.url).group (1)

        if self.mode == MODE_DOWNLOAD:
            self.tfile = TempFile (self.aurl, "dl.start=Free", self.url)
            self.tfile.completed_cb = self.stage_download_completed
            self.tfile.start ()
        else:
            self.state == STATE_PAUSED

    def handle_download_limit (self, match):
        m2 = re.search ('Or try again in about (\d+) minutes', self.tfile.contents)
        self.status = 'Download limit, try again in %s minutes' % (m2.group (1))
        self.state = STATE_WAITING

    def handle_already_downloading (self, match):
        self.status = 'You are already downloading %s' % (match.group (1))
        self.state = STATE_WAITING

    def handle_no_slots (self, match):
        self.status = 'No download slots available'
        self.state = STATE_WAITING

    def handle_no_slots_count (self, match):
        self.status = 'No download slots available, try again in %s minutes' % (match.group (1))
        self.state = STATE_WAITING

    def handle_download (self, match):
        m = re.search ('var c=(\d+)', self.tfile.contents)

        m2 = re.search ('<input checked type="radio" name="mirror" onclick="document\.dlf\.action=\\\\\'(.*?)\\\\\';" \/> (.*?)<br \/>', self.tfile.contents)
        print 'Mirror %s: %s' % (m2.group (2), m2.group (1))
        self.furl = m2.group (1)

        num = int (m.group (1))
        self.timeout = Timeout (num, self.start_download, self.print_progress)
        self.state = STATE_WAITING

    def start_download (self):
        m = re.search ('([^\/]*)$', self.furl)
        filename = m.group (1)

        self.tfile = WriteFile (self.furl, filename, None, self.url)
        self.tfile.completed_cb = self.download_completed
        self.tfile.progress_cb = self.download_progress
        self.tfile.start ()

        self.status = 'Downloading...'
        self.state = STATE_WAITING

import hosters
hosters.download_factory.add_hoster (Rapidshare, 'http:\/\/rapidshare\.com')
