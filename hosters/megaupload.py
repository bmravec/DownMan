import re

from tempfile import TempFile
from writefile import WriteFile
from timeout import Timeout
from generichost import GenericHost

class Megaupload (GenericHost):
    def __init__ (self, url, dm):
        GenericHost.__init__ (self, url, dm)

        self.case_handlers = [
            ('<FORM method="POST" id="captchaform">', self.handle_start_page),
            ('count=(\d+);', self.handle_correct_captcha),

        ]

    def stage_captcha_download_completed (self, tfile):
        self.code = self.dm.app.prompt_for_captcha (tfile.contents)

        if self.code != None:
            post_data = 'captchacode=%s&megavar=%s&captcha=%s' % (self.captchacode, self.megavar, self.code)

            self.tfile = TempFile (self.url, post_data)
            self.tfile.completed_cb = self.stage_download_completed
            self.tfile.start ()
        else:
            self.status = 'Invalid Code'

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

import hosters
hosters.factory.add_hoster (Megaupload, 'http:\/\/(www\.)?megaupload\.com\/\?d=')
