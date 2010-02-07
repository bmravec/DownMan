#
#       megaupload.py
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

from downman.downloaders.tempfile import TempFile
from downman.downloaders.writefile import WriteFile
from downman.downloaders.timeout import Timeout
from downman.downloaders.download import *
from generichost import *

MUDOWNLOAD_MATCH = 'http:\/\/(www\.)?megaupload\.com\/\?d='
MUDECRYPTOR_MATCH = 'http:\/\/(www\.)?megaupload\.com\/\?f='

class MUDownload (GenericHost):
    location = None

    def __init__ (self, url, downman):
        GenericHost.__init__ (self, url, downman)

        self.case_handlers = [
            ('<FORM method="POST" id="captchaform">', self.handle_start_page),
            ('count=(\d+);', self.handle_correct_captcha),
        ]

    def start_get_info (self):
        fileid = re.search ('\?d=([A-Za-z0-9]+)', self.url).group (1)
        postdata = 'id0=%s' % (fileid)

        self.tfile = TempFile ('http://www.megaupload.com/mgr_linkcheck.php', postdata, headers=['TE: trailers', 'Connection: TE'])
        self.tfile.completed_cb = self.handle_info_page
        self.tfile.start ()

    def handle_info_page (self, tfile):
        self.total = int (re.search ('s=(\d+)', self.tfile.contents).group (1))
        self.name = re.search ('n=(.+)', self.tfile.contents).group (1)

        self.set_state (STATE_INFO_COMPLETED)

    def handle_start_page (self, match):
        ccstr = '<INPUT type="hidden" name="captchacode" value="(.*?)">'
        m = re.search (ccstr, self.tfile.contents)
        self.captchacode = m.group (1)

        mvstr = '<INPUT type="hidden" name="megavar" value="(.*?)">'
        m = re.search (mvstr, self.tfile.contents)
        self.megavar = m.group (1)

        imgstr = '<TD>Enter this </TD>\s*?<TD width="100" align="center" height="40"><img src="([^"]*?)" border="0" alt=""></TD>\s*?<TD> here:</TD>'
        m = re.search (imgstr, self.tfile.contents)
        self.iurl = m.group (1)

        self.tfile = TempFile (self.iurl)
        self.tfile.completed_cb = self.stage_captcha_download_completed
        self.tfile.start ()

    def stage_captcha_download_completed (self, tfile):
        self.code = self.downman.application.prompt_for_captcha (tfile.contents)

        if self.code != None:
            post_data = 'captchacode=%s&megavar=%s&captcha=%s' % (self.captchacode, self.megavar, self.code)

            self.tfile = TempFile (self.url, post_data)
            self.tfile.completed_cb = self.stage_download_completed
            self.tfile.start ()
        else:
            self.status = 'Invalid Code'
            self.set_state (STATE_DISABLED)

    def handle_correct_captcha (self, match):
        num = int (match.group (1))

        m = re.search ('id="downloadlink"><a href="([^"]*)"', self.tfile.contents)
        self.furl = m.group (1)

        self.timeout = Timeout (num, self.handle_start_download, self.print_progress)
        self.set_state (STATE_WAITING)

    def handle_start_download (self):
        dpath = self.downman.config.get_property ('DefaultDownloadDirectory')

        self.location = os.path.join (dpath, re.search ('([^\/]*)$', self.furl).group (1))

        rfrom = None

        if os.path.exists (self.location):
            s = os.stat (self.location)

            if s.st_size == self.total:
                self.set_state (STATE_COMPLETED)
                return
            if s.st_size == self.downloaded:
                rfrom = self.downloaded

        self.tfile = WriteFile (self.furl, self.location, None, self.url, rfrom)
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
        data['match'] = MUDOWNLOAD_MATCH
        if self.location != None:
            data['location'] = self.location

        return data

class MUDecryptor (GenericHost):
    status = 'Unchecked'

    def __init__ (self, url, downman):
        self.url = url
        self.name = url
        self.total = 0

        self.folderid = re.search ('\?f=([A-Za-z0-9]+)', self.url).group (1)

    def start_get_info (self):
        self.tfile = TempFile ('http://megaupload.com/xml/folderfiles.php?folderid=%s' % (self.folderid))
        self.tfile.completed_cb = self.handle_tfile_done
        self.tfile.start ()

    def handle_tfile_done (self, tfile):
        rows = re.findall ('<ROW .*?</ROW>', tfile.contents)

        if len (rows) != 0:
            self.links = []

        for row in rows:
            self.links.append (re.search ('url="([^"]*)"', row).group (1))

        self.set_state (STATE_INFO_COMPLETED)

from downman.downloaders.hosters import factory
factory.add_hoster (MUDownload, MUDOWNLOAD_MATCH)
factory.add_hoster (MUDecryptor, MUDECRYPTOR_MATCH)
