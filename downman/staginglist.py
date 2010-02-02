#
#       staginglist.py
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

import downloaders

class StagingList:
    downloads = []
    view = None

    def __init__ (self, downman):
        self.downman = downman

    def add_download (self, download):
        self.downloads.append (download)

        if self.view != None:
            self.view.add_download (download)

        download.set_state_cb (self.on_info_cb)

        download.start_get_info ()

    def remove_download (self, download):
        self.downloads.remove (download)

        if self.view != None:
            self.view.remove_download (download)

        download.set_state_cb (None)

    def update_download (self, download):
        if self.view != None:
            self.view.update_download (download)

    def update_all (self):
        for d in self.downloads:
            self.update_download (d)

    def on_info_cb (self, download):
        if download.links != None:
            self.remove_download (download)

            for link in download.links:
                d = downloaders.create_download (link, self.downman)
                if d != None:
                    self.add_download (d)
        else:
            self.update_download (download)

    def clear (self):
        while len (self.downloads):
            self.remove_download (self.downloads[0])

    def get_all (self):
        ds = []

        for d in self.downloads:
            ds.append (d)

        return ds
