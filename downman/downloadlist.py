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

class DownloadList:
    downloads = []
    view = None

    downloading = None

    def __init__ (self, downman):
        self.downman = downman

    def add_download (self, download):
        self.downloads.append (download)

        if self.view != None:
            self.view.add_download (download)

        if len (self.downloads) == 1:
            download.start_download (self.handle_state_changed)
        else:
            download.set_state (STATE_QUEUED)

        #TODO: Handle Queue correctly

    def remove_download (self, download):
        self.downloads.remove (download)

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

        if download != self.downloading:
            return

        if download.state == STATE_COMPLETED or download.state == STATE_HOLDING or download.state == STATE_DISABLED or download.state == STATE_NOT_FOUND or download.state == STATE_PAUSED:
            found = False
            for d in self.downloads:
                if d == self.downloading:
                    found = True
                elif found == True:
                    d.start_download (self.handle_state_changed)
                    return

    def shutdown (self):
        dlist = []

        for d in self.downloads:
            ddict = d.shutdown ()
            if ddict != None:
                dlist.append (ddict)

        return dlist
