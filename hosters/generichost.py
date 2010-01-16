import re

from tempfile import TempFile
from writefile import WriteFile
from timeout import Timeout

class GenericHost:
    status = ''

    def __init__ (self, url, dm):
        self.url = url
        self.dm = dm
        self.downloaded = 0
        self.total = 0

        self.case_handlers = []

    def start (self):
        self.status = 'Connecting...'

        self.tfile = TempFile (self.url)
        self.tfile.completed_cb = self.stage_download_completed
        self.tfile.start ()

    def stage_download_completed (self, tfile):
        for handler in self.case_handlers:
            m = re.search (handler[0], self.tfile.contents)
            if m != None:
#                print 'Matched:', handler[0]
                handler[1] (m)

#        f = open ('output', 'wb')
#        f.write (self.tfile.contents)
#        f.close ()

    def download_completed (self, wfile):
        self.status = 'Download Complete'

    def download_progress (self, dt, dd, ut, ud):
        self.downloaded = dd
        self.total = dt

    def print_progress (self, num):
        self.status = 'Starting in %d seconds' % (num)
