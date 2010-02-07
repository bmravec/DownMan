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

from threading import Thread
import re, os, os.path
import pycurl

from downman.downloaders.tempfile import TempFile
from downman.downloaders.writefile import WriteFile
from downman.downloaders.timeout import Timeout
from generichost import *

RAPIDSHARE_MATCH = 'http:\/\/rapidshare\.com'

class Rapidshare (GenericHost):
    def __init__ (self, url, downman):
        GenericHost.__init__ (self, url, downman)

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
            self.total = self.total * 1000
        if m.group (2) == 'MB':
            self.total = self.total * 1000000
        if m.group (2) == 'GB':
            self.total = self.total * 1000000000

        self.name = re.search ('([^\/]*)$', self.url).group (1)

        if self.state != STATE_INFO:
            self.tfile = TempFile (self.aurl, "dl.start=Free", self.url)
            self.tfile.completed_cb = self.stage_download_completed
            self.tfile.start ()
        else:
            self.set_state (STATE_INFO_COMPLETED)

    def handle_download_limit (self, match):
        m2 = re.search ('Or try again in about (\d+) minutes', self.tfile.contents)
        self.status = 'Download limit, try again in %s minutes' % (m2.group (1))
        self.set_state (STATE_HOLDING)

    def handle_already_downloading (self, match):
        self.status = 'You are already downloading %s' % (match.group (1))
        self.set_state (STATE_HOLDING)

    def handle_no_slots (self, match):
        self.status = 'No download slots available'
        self.set_state (STATE_HOLDING)

    def handle_no_slots_count (self, match):
        self.status = 'No download slots available, try again in %s minutes' % (match.group (1))
        self.set_state (STATE_HOLDING)

    def handle_download (self, match):
        m = re.search ('var c=(\d+)', self.tfile.contents)

        m2 = re.search ('<input checked type="radio" name="mirror" onclick="document\.dlf\.action=\\\\\'(.*?)\\\\\';" \/> (.*?)<br \/>', self.tfile.contents)
#        print 'Mirror %s: %s' % (m2.group (2), m2.group (1))
        self.furl = m2.group (1)

        num = int (m.group (1))
        self.timeout = Timeout (num, self.handle_start_download, self.print_progress)
        self.set_state (STATE_WAITING)

    def handle_start_download (self):
        dpath = self.downman.config.get_property ('DefaultDownloadDirectory')

        self.location = os.path.join (dpath, re.search ('([^\/]*)$', self.furl).group (1))

        """
        resume = None

        if os.path.exists (self.location):
            s = os.stat (self.location)

            if s.st_size == self.total:
                self.set_state (STATE_COMPLETED)
                return
            if s.st_size == self.downloaded:
                self.furl = self.furl + '?start=' + str (int (self.downloaded))
                resume = self.downloaded
        """

        self.tfile = RapidshareWriteFile (self.furl, self.location, self.url)
        self.tfile.completed_cb = self.download_completed
        self.tfile.progress_cb = self.download_progress
        self.tfile.start ()

        self.status = 'Downloading...'
        self.set_state (STATE_DOWNLOADING)

    def startup (self, data):
        self.name = data['name']
        self.url = data['url']
        self.downloaded = float (data['downloaded'])
        self.total = float (data['total'])
        self.state = int (data['state'])
        if data.has_key ('location'):
            self.location = data['location']

    def shutdown (self):
        data = {}

        if self.state == STATE_CONNECTING:
            self.tfile.close ()
            self.state = STATE_QUEUED
        elif self.state == STATE_DOWNLOADING:
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
        data['match'] = RAPIDSHARE_MATCH
        if hasattr (self, 'location'):
            data['location'] = self.location

        return data

class RapidshareWriteFile (Thread):
    completed_cb = None
    progress_cb = None
    drun = False

    def __init__ (self, url, filename, referer=None, resume=None):
        Thread.__init__ (self)
        self.url = url
        self.filename = filename
        self.referer = referer
        self.resume = resume

    def run (self):
        self.c = pycurl.Curl ()
        self.c.setopt (pycurl.URL, self.url)
        self.c.setopt (pycurl.VERBOSE, 1)

        if self.resume != None:
            f = open (self.filename, 'ab')
        else:
            f = open (self.filename, 'wb')

        self.c.setopt (pycurl.WRITEFUNCTION, f.write)

        if self.referer != None:
            self.c.setopt (pycurl.REFERER, self.referer)

        self.c.setopt (pycurl.NOPROGRESS, 0)
        self.c.setopt (pycurl.PROGRESSFUNCTION, self.download_progress)

        self.drun = True

        try:
            try:
                self.c.perform ()
                if self.completed_cb != None:
                    self.completed_cb (self)
            except:
                pass
        finally:
            self.c.close ()
            f.close ()

    def download_progress (self, dt, dd, ut, ud):
        if self.resume:
            dd += self.resume

        if self.progress_cb:
            self.progress_cb (dt, dd, ut, ud)
        if not self.drun:
            return 1

    def close (self):
        self.drun = False
        self.join ()
        self.c.close ()

from downman.downloaders.hosters import factory
factory.add_hoster (Rapidshare, RAPIDSHARE_MATCH)
