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

from PyQt4 import QtGui

import gui.mainwindow

class MainWindow (gui.mainwindow.MainWindow, QtGui.QMainWindow):
    def __init__ (self, downman):
        QtGui.QMainWindow.__init__ (self)
        gui.mainwindow.MainWindow.__init__ (self, downman)

        self.setWindowTitle ('DownMan')
        self.setWindowIcon (QtGui.QIcon ('images/downman.svg'))
        self.resize (400, 300)

        self.tabwidget = QtGui.QTabWidget ()

        self.setCentralWidget (self.tabwidget)

        self.show ()

    def set_menubar (self, menubar):
        if self.menubar != None:
            self.table.removeWidget (self.menubar)

        self.table.add_widget (menubar, 0, 0)
        self.menubar = menubar

    def set_toolbar (self, toolbar):
        if self.toolbar != None:
            self.removeToolBar (self.toolbar)

        self.addToolBar (toolbar)
        self.toolbar = toolbar

    def set_downloadview (self, downloadview):
        return
        if self.downloadview != None:
            self.table.removeWidget (self.downloadview)

        self.table.addWidget (downloadview, 1, 0)
        self.downloadview = downloadview

    def set_stagingview (self, stagingview):
        return
        if self.stagingview != None:
            self.table.removeWidget (self.stagingview)

        self.table.addWidget (stagingview, 1, 0)
        self.stagingview = stagingview
