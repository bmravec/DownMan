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

import sys
from PyQt4 import QtGui

import gui.application

from mainwindow import MainWindow
from downloadview import DownloadView
from toolbar import Toolbar

class Application (gui.application.Application):
    def __init__ (self, downman):
        gui.application.Application.__init__ (self, downman)
        self.app = QtGui.QApplication (sys.argv)

    def run (self):
        self.app.exec_ ()

    def quit (self):
        sys.exit ()

    def prompt_for_captcha (self, imagedata):
        code = None

        return code

    def prompt_for_urls (self):
        text = None

        return text
