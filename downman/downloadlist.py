#
#       downloadlist.py
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

from downloaders.download import *

astates = [ STATE_CONNECTING, STATE_DOWNLOADING, STATE_WAITING ]
istates = [ STATE_NULL, STATE_PAUSED, STATE_HOLDING, STATE_DISABLED, STATE_COMPLETED, STATE_NOT_FOUND ]

class DownloadList:
    downloads = []
    view = None

    downloading = []

    def __init__ (self, downman):
        self.downman = downman

        self.maxnumdownloads = int (downman.config.get_property ('MaxNumDownloads'))
        downman.config.register_notifier ('MaxNumDownloads', self.on_key_change)

    def add_download (self, download):
        self.downloads.append (download)

        if self.view != None:
            self.view.add_download (download)

        # Sets up the callback, this will call it once
        # so we can setup the queue in the callback
        download.set_state_cb (self.handle_state_changed)

    def remove_download (self, download):
        self.downloads.remove (download)

        if download in self.downloading:
            self.downloading.remove (download)
            download.close ()

        if self.view != None:
            self.view.remove_download (download)

    def update_download (self, download):
        if self.view != None:
            self.view.update_download (download)

    def update_all (self):
        for d in self.downloads:
            self.update_download (d)

    def clear (self):
        while len (self.downloads):
            self.remove_download (self.downloads[0])

    def handle_state_changed (self, download):
        self.update_download (download)

        if download.state in istates and download in self.downloading:
            self.downloading.remove (download)

        self.refresh_queue ()

    def refresh_queue (self):
        slots = self.maxnumdownloads - len (self.downloading)

        if slots <= 0:
            return

        for d in self.downloads:
            if not d in self.downloading and d.state == STATE_QUEUED:
                self.downloading.append (d)
                d.start_download ()

                slots -= 1
                if slots <= 0:
                    return

    def on_key_change (self, key, value):
        if key == 'MaxNumDownloads':
            self.maxnumdownloads = int (value)
            self.refresh_queue ()

    def shutdown (self):
        dlist = []

        for d in self.downloads:
            ddict = d.shutdown ()
            if ddict != None:
                dlist.append (ddict)

        return dlist
