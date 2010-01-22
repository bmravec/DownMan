#
#       downman.py
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
import time
from threading import Thread

import gui
import downloaders

from downloadlist import DownloadList
from staginglist import StagingList

class DownMan:
    application = None
    downloadview = None
    stagingview = None
    mainwindow = None
    toolbar = None
    menubar = None

    downloadlist = None
    staginglist = None

    def __init__ (self):
        self.downloadlist = DownloadList (self)
        self.staginglist = StagingList (self)

        self.application = gui.Application (self)
        self.mainwindow = gui.MainWindow (self)
        self.toolbar = gui.Toolbar (self)
        self.downloadview = gui.DownloadView (self.downloadlist)
        self.stagingview = gui.StagingView (self.staginglist)

        self.mainwindow.set_toolbar (self.toolbar)
        self.mainwindow.set_downloadview (self.downloadview)
        self.mainwindow.set_stagingview (self.stagingview)

        self.updater = TimeoutPing (1, self.update_all)
        self.updater.start ()

    def remove_download (self, download):
        self.app.remove_download (download)
        self.downloads.remove (download)

        download.close ()

    def update_download (self, download):
        self.staginglist.update_download (download)
#        self.downloadlist.update_download (download)

    def update_all (self):
        self.staginglist.update_all ()
#        self.downloadlist.update_all ()

    def update_queue (self):
        for d in self.downloads:
            d.start ()

    def on_add_url (self):
        text = self.application.prompt_for_urls ()

        if text == None:
            return

        m = re.findall ('((https?|ftps?)://[^\s]*)', text)

        for i in m:
            download = downloaders.create_download (i[0])
            if download != None:
                self.staginglist.add_download (download)

    def on_add_file (self):
        print 'DownMan.on_add_file (): stub'

    def on_remove (self):
        print 'DownMan.on_remove (): stub'

    def on_start (self):
        print 'DownMan.on_start (): stub'

    def on_stop (self):
        print 'DownMan.on_stop (): stub'

    def on_start_staging (self):
        print 'DownMan.on_start_staging (): stub'

    def on_clear_staging (self):
        self.staginglist.clear ()

class TimeoutPing (Thread):
    def __init__ (self, interval, callback):
        Thread.__init__ (self)
        self.interval = interval
        self.callback = callback

    def run (self):
        while True:
            time.sleep (self.interval)
            if self.callback () != None:
                break
