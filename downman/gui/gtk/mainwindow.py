#
#       mainwindow.py
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
import time
import gobject

from .. import mainwindow

class MainWindow (gtk.Window, mainwindow.MainWindow):
    def __init__ (self, downman):
        gtk.Window.__init__ (self, gtk.WINDOW_TOPLEVEL)
        mainwindow.MainWindow.__init__ (self, downman)

        self.set_default_size (400, 300)
        self.connect ("destroy", self.destroy)

        self.set_icon (gtk.gdk.pixbuf_new_from_file ('images/downman.svg'))
        self.set_title ('DownMan')

        self.table = gtk.Table (4)
        self.add (self.table)

        self.notebook = gtk.Notebook ()
        self.table.attach (self.notebook, 0, 1, 2, 3)

        self.downloadview_sw = gtk.ScrolledWindow ()
        self.stagingview_sw = gtk.ScrolledWindow ()

        self.downloadview_sw.set_policy (gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC)
        self.stagingview_sw.set_policy (gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC)

        self.notebook.insert_page (self.downloadview_sw, gtk.Label ('Downloads'), 0)
        self.notebook.insert_page (self.stagingview_sw, gtk.Label ('Staging'), 1)

        self.show_all ()

    def destroy (self, widget):
        self.downman.application.quit ()

    def set_toolbar (self, toolbar):
        if self.toolbar != None:
            self.table.remove (self.toolbar)

        self.table.attach (toolbar, 0, 1, 1, 2, yoptions=0)
        self.toolbar = toolbar

    def set_downloadview (self, downloadview):
        if self.downloadview != None:
            self.downloadview_sw.remove (self.downloadview)

        self.downloadview_sw.add (downloadview)
        self.downloadview = downloadview

    def set_stagingview (self, stagingview):
        if self.stagingview != None:
            self.stagingview_sw.remove (self.stagingview)

        self.stagingview_sw.add (stagingview)
        self.stagingview = stagingview

    def set_menubar (self, menubar):
        if self.menubar != None:
            self.table.remove (self.menubar)

        self.table.attach (menubar, 0, 1, 0, 1, yoptions=0)
        self.menubar = menubar
