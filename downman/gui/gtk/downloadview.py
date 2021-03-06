#
#       downloadview.py
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

import gtk

from .. import downloadview

from downman.utils import *

from downman.downloaders.download import STATE_DOWNLOADING

class DownloadView (gtk.TreeView, downloadview.DownloadView):
    def __init__ (self, downloadlist):
        gtk.TreeView.__init__ (self)
        downloadview.DownloadView.__init__ (self, downloadlist)

        self.store = gtk.TreeStore (object, str, str, str, float, int)
        self.set_model (self.store)

        self.get_selection ().set_mode (gtk.SELECTION_MULTIPLE)

        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Name', cell, text=1)
        column.set_expand (True)
        self.append_column (column)

        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Status', cell, text=2)
        self.append_column (column)

        cell = gtk.CellRendererProgress ()
        column = gtk.TreeViewColumn ('Progress', cell, text=3, value=4, pulse=5)
        column.set_expand (True)
        self.append_column (column)

        self.show_all ()

    def add_download (self, download):
        if download.total != -1:
            pulse = -1

            if download.total != 0:
                if download.downloaded >= download.total:
                    value = 100.0
                else:
                    value = 100.0 * download.downloaded / download.total

                text = '%s / %s' % (size_to_string (download.downloaded), size_to_string (download.total))
            else:
                value = 0.0
                text = '%s' % (size_to_string (download.downloaded))
        else:
            text = '%d' % (size_to_string (download.downloaded))
            pulse = download.downloaded
            value = 0.0

        iter = self.store.append (None, row=(download, download.name, download.status, text, value, pulse))

    def update_download (self, download):
        for d in self.store:
            if d[0] == download:
                d[1] = download.name

                if download.state == STATE_DOWNLOADING:
                    dtext = None
                    utext = None

                    if download.speed.putotal != 0:
                        utext = 'U: %s' % (size_to_string (download.speed.putotal))

                    if download.speed.pdtotal != 0:
                        dtext = 'D: %s' % (size_to_string (download.speed.pdtotal))

                    if dtext and utext:
                        d[2] = '%sps, %sps' % (utext, dtext)
                    elif utext:
                        d[2] = '%sps' % (utext)
                    elif dtext:
                        d[2] = '%sps' % (dtext)
                else:
                    d[2] = download.status

                if download.total != -1:
                    if download.total != 0:
                        if download.downloaded >= download.total:
                            d[4] = 100.0
                        else:
                            d[4] = 100.0 * download.downloaded / download.total

                        d[3] = '%s / %s' % (size_to_string (download.downloaded), size_to_string (download.total))
                    else:
                        d[4] = 0.0
                        d[3] = '%s' % (size_to_string (download.downloaded))
                else:
                    d[5] = download.downloaded
                    d[3] = '%s' % (size_to_string (download.downloaded))

    def remove_download (self, download):
        for d in self.store:
            if d[0] == download:
                self.store.remove (d.iter)
                return

    def get_selected (self):
        ds = []

        model, rows = self.get_selection ().get_selected_rows ()

        for row in rows:
            ds.append (model[row][0])

        return ds
