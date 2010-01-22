#
#       toolbar.py
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

import gui.toolbar

class Toolbar (gtk.Toolbar, gui.toolbar.Toolbar):
    def __init__ (self, downman):
        gtk.Toolbar.__init__ (self)
        gui.toolbar.Toolbar.__init__ (self, downman)

        self.add_url_button = gtk.ToolButton (gtk.STOCK_NETWORK)
        self.add_url_button.set_tooltip_text ('Add URL')
        self.add_url_button.connect ('clicked', self.add_url_clicked)
        self.insert (self.add_url_button, -1)

        self.add_file_button = gtk.ToolButton (gtk.STOCK_OPEN)
        self.add_file_button.set_tooltip_text ('Add File')
        self.add_file_button.connect ('clicked', self.add_file_clicked)
        self.insert (self.add_file_button, -1)

        self.remove_button = gtk.ToolButton (gtk.STOCK_REMOVE)
        self.remove_button.set_tooltip_text ('Remove Selected')
        self.remove_button.connect ('clicked', self.remove_clicked)
        self.insert (self.remove_button, -1)

        sep = gtk.SeparatorToolItem ()
        self.insert (sep, -1)

        self.start_button = gtk.ToolButton (gtk.STOCK_MEDIA_PLAY)
        self.start_button.set_tooltip_text ('Start')
        self.start_button.connect ('clicked', self.start_clicked)
        self.insert (self.start_button, -1)

        self.stop_button = gtk.ToolButton (gtk.STOCK_MEDIA_STOP)
        self.stop_button.set_tooltip_text ('Stop')
        self.stop_button.connect ('clicked', self.stop_clicked)
        self.insert (self.stop_button, -1)

        sep = gtk.SeparatorToolItem ()
        self.insert (sep, -1)

        self.start_staging_button = gtk.ToolButton (gtk.STOCK_MEDIA_NEXT)
        self.start_staging_button.set_tooltip_text ('Start Staging Downloads')
        self.start_staging_button.connect ('clicked', self.start_staging_clicked)
        self.insert (self.start_staging_button, -1)

        self.clear_staging_button = gtk.ToolButton (gtk.STOCK_CLEAR)
        self.clear_staging_button.set_tooltip_text ('Clear Staging Area')
        self.clear_staging_button.connect ('clicked', self.clear_staging_clicked)
        self.insert (self.clear_staging_button, -1)

        self.show_all ()

    def add_url_clicked (self, item):
        self.downman.on_add_url ()

    def add_file_clicked (self, item):
        self.downman.on_add_file ()

    def remove_clicked (self, item):
        self.downman.on_remove ()

    def start_clicked (self, item):
        self.downman.on_start ()

    def stop_clicked (self, item):
        self.downman.on_stop ()

    def start_staging_clicked (self, item):
        self.downman.on_start_staging ()

    def clear_staging_clicked (self, item):
        self.downman.on_clear_staging ()

    def set_start_enabled (self, val):
        self.start_button.set_sensitive (val)

    def set_stop_enabled (self, val):
        self.stop_button.set_sensitive (val)
