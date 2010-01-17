import re

from tempfile import TempFile
from writefile import WriteFile
from timeout import Timeout

STATE_PAUSED = 0
STATE_QUEUED = 1
STATE_CONNECTING = 2
STATE_DOWNLOADING = 3
STATE_DISABLED = 4
STATE_WAITING = 5
STATE_HOLDING = 6
STATE_DONE = 7

MODE_INFO = 0
MODE_DOWNLOAD = 1

class GenericHost:
    status = ''
    state = STATE_QUEUED
    mode = MODE_DOWNLOAD

    def __init__ (self, url, dm):
        self.url = url
        self.dm = dm
        self.downloaded = 0
        self.total = 0
        self.name = url

        self.case_handlers = []

    def start (self, mode=MODE_INFO):
        if mode == MODE_INFO:
            self.mode = MODE_INFO

            self.tfile = TempFile (self.url)
            self.tfile.completed_cb = self.stage_download_completed
            self.tfile.start ()
            return

        if self.state == STATE_PAUSED or self.state == STATE_DISABLED or self.state == STATE_DONE:
            return

        for d in self.dm.downloads:
            if d == self:
                break

            if isinstance (d, self.__class__):
                print 'd.state =',d.state
                if d.state == STATE_DOWNLOADING or d.state == STATE_HOLDING or d.state == STATE_WAITING or d.state == STATE_CONNECTING:
                    self.state = STATE_HOLDING
                    self.status = 'Queued'
                    return

        self.status = 'Connecting...'
        self.state = STATE_CONNECTING

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
        self.state = STATE_DONE
        self.dm.update_queue ()

    def download_progress (self, dt, dd, ut, ud):
        self.downloaded = dd
        self.total = dt

    def print_progress (self, num):
        self.status = 'Starting in %d seconds' % (num)
