#
#       speedlimit.py
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

import time
from threading import Thread

class DownloadLimit:
    ulimit = -1
    dlimit = -1

    utotal = -1
    dtotal = -1

    putotal = -1
    pdtotal = -1

    def __init__ (self, gl):
        self.gl = gl

    def update_downloaded (self, downloaded):
        self.dtotal += downloaded

        val = self.gl.update_downloaded (downloaded)
        if val != None:
            return val

        if self.dlimit != -1:
            #TODO: Handle individual download limits
            pass

    def update_uploaded (self, uploaded):
        self.utotal += uploaded

        val = self.gl.update_uploaded (downloaded)
        if val != None:
            return val

        if self.ulimit != -1:
            #TODO: Handle individual download limits
            pass

    def update (self):
        self.putotal = self.utotal
        self.pdtotal = self.dtotal

        self.utotal = 0
        self.dtotal = 0

    def wait (self, val):
        if val != None and val > 0:
            time.sleep (val)

class SpeedLimit (Thread):
    ulimit = -1
    dlimit = -1

    utotal = 0
    dtotal = 0

    putotal = 0
    pdtotal = 0

    last_time = 0.0

    downloads = []

    update_cb = None

    def __init__ (self, downman, update_cb=None):
        Thread.__init__ (self)

        self.downman = downman
        self.update_cb = update_cb

        self.ulimit = int (self.downman.config.get_property ('MaxUploadSpeed'))
        self.dlimit = int (self.downman.config.get_property ('MaxDownloadSpeed'))

        self.downman.config.register_notifier ('MaxUploadSpeed', self.update_settings)
        self.downman.config.register_notifier ('MaxDownloadSpeed', self.update_settings)

    def run (self):
        self.last_time = time.time ()

        while True:
            self.putotal = self.utotal
            self.pdtotal = self.dtotal

            self.utotal = 0
            self.dtotal = 0

            for d in self.downloads:
                d.update ()

            self.last_time = time.time ()

            if self.update_cb != None:
                self.update_cb ()

            diff = 1.0 - time.time () + self.last_time

            time.sleep (diff)

    def create_download (self):
        dl = DownloadLimit (self)

        self.downloads.append (dl)

        return dl

    def remove_download (self, dl):
        self.downloads.remove (dl)

    def update_downloaded (self, downloaded):
        self.dtotal += downloaded

        if self.dlimit != -1:
            if self.dtotal >= self.dlimit:
                diff = 1.0 - time.time () + self.last_time
                return diff

    def update_uploaded (self, uploaded):
        self.utotal += uploaded

        if self.ulimit != -1:
            if self.utotal >= self.ulimit:
                diff = 1.0 - time.time () + self.last_time
                return diff

    def update_settings (self, key, val):
        if key == 'MaxUploadSpeed':
            self.ulimit = int (self.downman.config.get_property ('MaxUploadSpeed'))
        elif key == 'MaxDownloadSpeed':
            self.dlimit = int (self.downman.config.get_property ('MaxDownloadSpeed'))
