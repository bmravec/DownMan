#
#       application.py
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

from mainwindow import MainWindow
from downloadview import DownloadView
from toolbar import Toolbar

gtk.gdk.threads_init ()

class Application:
    def __init__ (self, downman):
        self.downman = downman

        self.win = MainWindow (self)

        self.toolbar = Toolbar (self)
        self.toolbar.show_all ()
        self.win.pack_widget (self.toolbar, False)

        self.dv = DownloadView (self)
        scrolled = gtk.ScrolledWindow ()
        scrolled.add (self.dv)
        scrolled.set_policy (gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC)
        scrolled.show_all ()
        self.win.pack_widget (scrolled)

    def run (self):
        gtk.main ()

    def quit (self):
        gtk.main_quit ()

    def prompt_for_captcha (self, imagedata):
        code = None

        gtk.gdk.threads_enter ()

        pbloader = gtk.gdk.PixbufLoader ()
        pbloader.write (imagedata)
        pbloader.close ()

        pixbuf = pbloader.get_pixbuf ()

        img = gtk.image_new_from_pixbuf (pixbuf)

        entry = gtk.Entry ()

        dialog = gtk.Dialog ('Enter Captcha Code...',
            buttons=('Cancel', gtk.RESPONSE_CANCEL, 'Ok', gtk.RESPONSE_OK))

        entry.connect ('activate', self.prompt_entry_activated, dialog)

        ca = dialog.get_content_area ()
        ca.pack_start (img)
        ca.pack_start (entry)
        ca.show_all ()

        rid = dialog.run ()

        if rid == gtk.RESPONSE_OK:
            code = entry.get_text ()

        dialog.destroy ()

        gtk.gdk.threads_leave ()

        return code

    def prompt_entry_activated (self, entry, dialog):
        dialog.response (gtk.RESPONSE_OK)

    def add_download (self, download):
        self.dv.add_download (download)

    def update_download (self, download):
        self.dv.update_download (download)

    def remove_download (self, download):
        self.dv.remove_download (download)
