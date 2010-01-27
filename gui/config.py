#
#       config.py
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

DEFAULT_DOWNLOAD_DIRECTORY = None
DEFAULT_CONFIG_DIRECTORY = None

from xml.sax import make_parser, handler
import os.path

STATE_NULL = 0
STATE_DOWNMAN = 1
STATE_CONFIG = 2
STATE_CONFIG_PROPERTY = 3
STATE_DOWNLOAD = 4
STATE_DOWNLOAD_PROPERTY = 5
STATE_ERROR = 6

class ConfigSaxHandler (handler.ContentHandler):
    state = STATE_NULL
    tdownload = None
    key = ''
    value = ''

    def __init__ (self, config):
        self.config = config

    def startElement (self, name, attrs):
        if self.state == STATE_NULL:
            if name == 'downman':
                self.state = STATE_DOWNMAN
        elif self.state == STATE_DOWNMAN:
            if name == 'config':
                self.state = STATE_CONFIG
            elif name == 'download':
                self.state = STATE_DOWNLOAD
                self.tdownload = {}
        elif self.state == STATE_CONFIG:
            self.key = name
            self.state = STATE_CONFIG_PROPERTY
        elif self.state == STATE_DOWNLOAD:
            self.key = name
            self.state = STATE_DOWNLOAD_PROPERTY

    def endElement (self, name):
        if self.state == STATE_DOWNMAN:
            self.state = STATE_NULL
        elif self.state == STATE_CONFIG:
            self.state = STATE_DOWNMAN
        elif self.state == STATE_CONFIG_PROPERTY:
            self.state = STATE_CONFIG
            if self.key == name:
                self.config.set_property (self.key, self.value)
            else:
                self.state = STATE_ERROR
                print 'Invalid Xml file'
        elif self.state == STATE_DOWNLOAD:
            self.state = STATE_DOWNMAN
            self.config.tdownloads.append (self.tdownload)
        elif self.state == STATE_DOWNLOAD_PROPERTY:
            self.state = STATE_DOWNLOAD
            self.tdownload[self.key] = self.value

    def characters (self, content):
        if self.state == STATE_DOWNLOAD_PROPERTY or self.state == STATE_CONFIG_PROPERTY:
            self.value = content

class Config:
    '''Configuration and state handling class

    This class handles the configuration and state between runs of the
    application.  Classes can register to be notified when a key changes value.
    This makes it possible for changes in the configuration to be instantly
    reflected in all other parts of the application.  This class also stores the
    location for a directory to store state information like download progress.
    All downloads and a config file will be stored in that location so on the
    next load, all the configuration and downloads will be what they were when
    the application was last closed.

    Here is the list of all keys available for the config section
    and a short description of their function:
    MaxUploadSpeed - Global maximum upload speed of all transfers
    MaxDownloadSpeed - Global maxium download speed of all transfers
    DefaultDownloadDirectory - Default directory to download files to
    '''
    properties = {}
    notifiers = {}
    tdownloads = []

    def __init__ (self):
        self.set_property ('DefaultDownloadDirectory', DEFAULT_DOWNLOAD_DIRECTORY)
        self.set_property ('MaxUploadSpeed', '0')
        self.set_property ('MaxDownloadSpeed', '0')

    def get_property (self, key):
        return self.properties[key]

    def set_property (self, key, value):
        if not self.properties.has_key (key):
            self.notifiers[key] = []

        self.properties[key] = value

        for cb in self.notifiers[key]:
            cb (key, value)

    def register_notifier (self, key, handler):
        if not self.properties.has_key (key):
            self.notifiers[key] = []

        cbs = self.notifiers[key]
        cbs.append (handler)

    def remove_notifier (self, key, handler):
        cbs = self.notifiers[key]

        for cb in cbs:
            if cb == handler:
                cbs.remove (handler)
                break

    def load_settings (self):
        filename = os.path.join (DEFAULT_CONFIG_DIRECTORY, 'config.xml')

        if os.path.isfile (filename):
            parser = make_parser ()
            parser.setContentHandler (ConfigSaxHandler (self))
            parser.parse (filename)

    def load_downloads (self):
        print 'Config.load_downloads (): stub'

    def save (self):
        if not os.path.exists (DEFAULT_CONFIG_DIRECTORY):
            os.mkdir (DEFAULT_CONFIG_DIRECTORY)

        filename = os.path.join (DEFAULT_CONFIG_DIRECTORY, 'config.xml')
