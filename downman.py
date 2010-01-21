#!/usr/bin/python
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
"""
import sys
import time
from threading import Thread
import re

import hosters
from gui.application import Application
from gui.mainwindow import MainWindow

class DownMan:
    downloads = []
    decryptors = []

    def __init__ (self):
        self.app = Application (self)

        self.updater = TimeoutPing (1, self.update_all)
        self.updater.start ()

    def create_downloads (self, text):
        ds = []

        if isinstance (text, str):
            m = re.findall ('((https?|ftps?)://[^\s]*)', text)

            for i in m:
                hoster = hosters.download_factory.create_host_object (i[0], self)
                if hoster != None:
                    ds.append (hoster)
                    continue

                decryptor = hosters.decryptor_factory.create_host_object (i[0], self)
                if decryptor != None:
                    ds.append (decryptor)
                    continue
        else:
            for i in text:
                hoster = hosters.download_factory.create_host_object (i, self)
                if hoster != None:
                    ds.append (hoster)
                    continue

                decryptor = hosters.decryptor_factory.create_host_object (i, self)
                if decryptor != None:
                    ds.append (decryptor)
                    continue

        return ds

    def parse_download (self, text):
        m = re.findall ('((https?|ftps?)://[^\s]*)', text)
        for i in m:
            self.add_download (i[0])

    def add_download (self, download, start=False):
        if isinstance (download, str):
            hoster = hosters.download_factory.create_host_object (download, self)

            if hoster != None:
                self.downloads.append (hoster)
                self.app.add_download (hoster)

                hoster.start ()
                return

            decryptor = hosters.decryptor_factory.create_host_object (download, self)

            if decryptor != None:
                self.decryptors.append (decryptor)

                decryptor.start ()
                return

            print 'Cannot download from ', url
        else:
            self.downloads.append (download)
            self.app.add_downloads (download)

    def remove_download (self, download):
        self.app.remove_download (download)
        self.downloads.remove (download)

        download.close ()

    def update_download (self, download):
        self.app.update_download (download)

    def update_all (self):
        for d in self.downloads:
            self.update_download (d)

    def update_queue (self):
        for d in self.downloads:
            d.start ()

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
"""
import sys
from downman.downman import DownMan

if __name__ == '__main__':
    dm = DownMan ()

    dm.application.run ()
    sys.exit ()
