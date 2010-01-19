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

import re

from tempfile import TempFile
from writefile import WriteFile
from timeout import Timeout
from generichost import *

class MUDownload (GenericHost):
    def __init__ (self, url, dm):
        GenericHost.__init__ (self, url, dm)

        self.case_handlers = [
            ('<FORM method="POST" id="captchaform">', self.handle_start_page),
            ('count=(\d+);', self.handle_correct_captcha),
        ]

    def start_mode_info (self):
        fileid = re.search ('\?d=([A-Za-z0-9]+)', self.url).group (1)
        postdata = 'id0=%s' % (fileid)

        self.tfile = TempFile ('http://www.megaupload.com/mgr_linkcheck.php', postdata, headers=['TE: trailers', 'Connection: TE'])
        self.tfile.completed_cb = self.handle_info_page
        self.tfile.start ()

    def handle_info_page (self, tfile):
        self.total = int (re.search ('s=(\d+)', self.tfile.contents).group (1))
        self.name = re.search ('n=(.+)', self.tfile.contents).group (1)

        self.state = STATE_PAUSED

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
        self.code = self.dm.app.prompt_for_captcha (tfile.contents)

        if self.code != None:
            post_data = 'captchacode=%s&megavar=%s&captcha=%s' % (self.captchacode, self.megavar, self.code)

            self.tfile = TempFile (self.url, post_data)
            self.tfile.completed_cb = self.stage_download_completed
            self.tfile.start ()
        else:
            self.status = 'Invalid Code'

    def handle_correct_captcha (self, match):
        num = int (match.group (1))

        m = re.search ('id="downloadlink"><a href="([^"]*)"', self.tfile.contents)
        self.furl = m.group (1)

        self.timeout = Timeout (num, self.start_download, self.print_progress)

    def start_download (self):
        m = re.search ('([^\/]*)$', self.furl)
        filename = m.group (1)

        self.tfile = WriteFile (self.furl, filename, None, self.url)
        self.tfile.completed_cb = self.download_completed
        self.tfile.progress_cb = self.download_progress
        self.tfile.start ()

        self.status = 'Downloading...'
        self.state = STATE_DOWNLOADING

class MUDecryptor:
    def __init__ (self, url, dm):
        self.url = url
        self.downman = dm

        self.folderid = re.search ('\?f=([A-Za-z0-9]+)', self.url).group (1)

    def start (self):
        self.tfile = TempFile ('http://megaupload.com/xml/folderfiles.php?folderid=%s' % (self.folderid))
        self.tfile.completed_cb = self.handle_tfile_done
        self.tfile.start ()

    def handle_tfile_done (self, tfile):
        rows = re.findall ('<ROW .*?</ROW>', tfile.contents)
        for row in rows:
            name = re.search ('name="([^"]*)"', row).group (1)
            url = re.search ('url="([^"]*)"', row).group (1)
            total = int (re.search ('sizeinbytes="([^"]*)"', row).group (1))

            download = MUDownload (url, self.downman)
            download.total = total
            download.name = name

import hosters
hosters.download_factory.add_hoster (MUDownload, 'http:\/\/(www\.)?megaupload\.com\/\?d=')
hosters.decryptor_factory.add_hoster (MUDecryptor, 'http:\/\/(www\.)?megaupload\.com\/\?f=')
