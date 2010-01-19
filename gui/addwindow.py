#
#       addwindow.py
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

class AddWindow (gtk.Dialog):
    def __init__ (self):
        gtk.Dialog.__init__ (self, 'Add Downloads', buttons=('Cancel', gtk.RESPONSE_CANCEL, 'Start', gtk.RESPONSE_OK))

        ca = self.get_content_area ()

        hbox = gtk.HBox ()

        self.aurl_button = gtk.Button ('Add URL')
        self.afile_button = gtk.Button ('Add File')

        hbox.pack_start (self.aurl_button, expand=False)
        hbox.pack_start (self.afile_button, expand=False)

        ca.pack_start (hbox, expand=False)

        self.store = gtk.ListStore (object, str, int, str)
        self.view = gtk.TreeView (self.store)

        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Name', cell, text=1)
        column.set_expand (True)
        self.view.append_column (column)

        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Size', cell, text=2)
        self.view.append_column (column)

        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Status', cell, text=3)
        self.view.append_column (column)

        sw = gtk.ScrolledWindow ()
        sw.set_policy (gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC)
        sw.add (self.view)
        ca.pack_start (sw)

        self.notebook = gtk.Notebook ()

        self.filesbox = gtk.VBox ()
        self.notebook.append_page (self.filesbox, gtk.Label ('File(s)'))

        self.optionsbox = gtk.VBox ()
        self.notebook.append_page (self.optionsbox, gtk.Label ('Options'))

        self.notebook.set_sensitive (False)

        ca.pack_start (self.notebook, expand=False)

        self.start_check = gtk.CheckButton ('Start on adding')
        self.top_check = gtk.CheckButton ('Add to top of list')

        ca.pack_start (self.start_check, expand=False)
        ca.pack_start (self.top_check, expand=False)

        ca.show_all ()

    def run (self):
        rid = gtk.Dialog.run (self)
        downloads = []

        if rid == gtk.RESPONSE_OK:
            for d in self.store:
                downloads.append (d)

        return downloads

    def fill_files (self, download):
        self.filesbox.foreach (self.filesbox.remove)
        self.fill_files_funcs[download.proto] (download)

    def fill_files_http (self, download):
        pass
