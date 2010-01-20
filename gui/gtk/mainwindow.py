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

class MainWindow (gtk.Window):
    prev_progress = None
    prev_downloaded = None

    def __init__ (self, app):
        gtk.Window.__init__ (self, gtk.WINDOW_TOPLEVEL)

        self.app = app

        self.set_default_size (400, 300)
        self.connect ("destroy", self.destroy)

        self.vbox = gtk.VBox ()
        self.add (self.vbox)

        self.show_all ()

    def destroy (self, widget):
        self.app.quit ()

    def pack_widget (self, widget, expand=True):
        self.vbox.pack_start (widget, expand=expand)
