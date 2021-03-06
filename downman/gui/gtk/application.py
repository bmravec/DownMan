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
import sys

from .. import application

from mainwindow import MainWindow
from downloadview import DownloadView
from toolbar import Toolbar

gtk.gdk.threads_init ()

class Application (application.Application):
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

    def prompt_for_urls (self):
        text = None

        textview = gtk.TextView ()
        dialog = gtk.Dialog ('Add URLs...',
            buttons=('Cancel', gtk.RESPONSE_CANCEL, 'Ok', gtk.RESPONSE_OK))

        ca = dialog.get_content_area ()
        ca.pack_start (gtk.Label ('Enter URLs below:'), expand=False)

        sw = gtk.ScrolledWindow ()
        sw.set_policy (gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
        sw.add (textview)
        ca.pack_start (sw)
        ca.show_all ()

        if dialog.run () == gtk.RESPONSE_OK:
            textbuf = textview.get_buffer ()
            text = textbuf.get_text (textbuf.get_start_iter (), textbuf.get_end_iter ())

        dialog.destroy ()

        return text
