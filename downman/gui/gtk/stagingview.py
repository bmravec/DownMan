#
#       stagingview.py
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

from .. import stagingview

from downman.utils import *

class StagingView (gtk.TreeView, stagingview.StagingView):
    def __init__ (self, staginglist):
        gtk.TreeView.__init__ (self)
        stagingview.StagingView.__init__ (self, staginglist)

        self.store = gtk.TreeStore (object, str, str, str)
        self.set_model (self.store)

        self.get_selection ().set_mode (gtk.SELECTION_MULTIPLE)

        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Name', cell, text=1)
        column.set_expand (True)
        self.append_column (column)

        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Total', cell, text=2)
        self.append_column (column)

        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Status', cell, text=3)
#        column.set_expand (True)
        self.append_column (column)

        self.show_all ()

    def add_download (self, download):
        iter = self.store.append (None, row=(download, download.name, size_to_string (download.total), 'Unknown'))

    def update_download (self, download):
        for d in self.store:
            if d[0] == download:
                d[1] = download.name
                d[2] = size_to_string (download.total)
                d[3] = download.status

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
