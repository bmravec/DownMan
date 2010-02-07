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

from PyQt4 import QtGui, QtCore

import gui.toolbar

class Toolbar (gui.toolbar.Toolbar, QtGui.QToolBar):
    def __init__ (self, downman):
        QtGui.QToolBar.__init__ (self)
        gui.toolbar.Toolbar.__init__ (self, downman)

        style = downman.application.app.style ()

        icon = style.standardIcon (QtGui.QStyle.SP_DriveNetIcon)
        self.add_url_action = self.addAction (icon, 'Add Url')
        self.connect (self.add_url_action, QtCore.SIGNAL ('triggered ()'), self.add_url_clicked)

        icon = style.standardIcon (QtGui.QStyle.SP_FileIcon)
        self.add_file_action = self.addAction (icon, 'Add File')
        self.connect (self.add_file_action, QtCore.SIGNAL ('triggered ()'), self.add_file_clicked)

        icon = style.standardIcon (QtGui.QStyle.SP_TrashIcon)
        self.remove_action = self.addAction (icon, 'Remove Selected')
        self.connect (self.remove_action, QtCore.SIGNAL ('triggered ()'), self.remove_clicked)

        self.addSeparator ()

        icon = style.standardIcon (QtGui.QStyle.SP_MediaPlay)
        self.start_action = self.addAction (icon, 'Start')
        self.connect (self.start_action, QtCore.SIGNAL ('triggered ()'), self.start_clicked)

        icon = style.standardIcon (QtGui.QStyle.SP_MediaStop)
        self.stop_action = self.addAction (icon, 'Stop')
        self.connect (self.stop_action, QtCore.SIGNAL ('triggered ()'), self.stop_clicked)

        self.addSeparator ()

        icon = style.standardIcon (QtGui.QStyle.SP_DialogApplyButton)
        self.start_staging_action = self.addAction (icon, 'Start Staging')
        self.connect (self.start_staging_action, QtCore.SIGNAL ('triggered ()'), self.start_staging_clicked)

        icon = style.standardIcon (QtGui.QStyle.SP_DialogDiscardButton)
        self.clear_staging_action = self.addAction (icon, 'Clear Staging')
        self.connect (self.clear_staging_action, QtCore.SIGNAL ('triggered ()'), self.clear_staging_clicked)


    def add_url_clicked (self):
        self.downman.on_add_url ()

    def add_file_clicked (self):
        self.downman.on_add_file ()

    def remove_clicked (self):
        self.downman.on_remove ()

    def start_clicked (self):
        self.downman.on_start ()

    def stop_clicked (self):
        self.downman.on_stop ()

    def start_staging_clicked (self):
        self.downman.on_start_staging ()

    def clear_staging_clicked (self):
        self.downman.on_clear_staging ()

    def set_start_enabled (self, val):
        self.start_action.setEnabled (val)

    def set_stop_enabled (self, val):
        self.stop_action.setEnabled (val)
