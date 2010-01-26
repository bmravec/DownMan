#
#       __init__.py
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

DEFAULT_GUI = 'gtk'

import os.path
import config
from config import Config

if DEFAULT_GUI == 'gtk':
    from gtk.application import Application
    from gtk.mainwindow import MainWindow
    from gtk.toolbar import Toolbar
    from gtk.downloadview import DownloadView
    from gtk.stagingview import StagingView
    config.DEFAULT_DOWNLOAD_DIRECTORY = os.path.expanduser ('~/Downloads')
    config.DEFAULT_CONFIG_DIRECTORY = os.path.expanduser ('~/.config/downman')
elif DEFAULT_GUI == 'qt':
    from qt.application import Application
    from qt.mainwindow import MainWindow
    from qt.toolbar import Toolbar
    from qt.downloadview import DownloadView
    from qt.stagingview import StagingView
else:
    from application import Application
    from mainwindow import MainWindow
    from toolbar import Toolbar
    from downloadview import DownloadView
    from stagingview import StagingView
