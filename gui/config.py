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

    Here is a tentative list of all keys available and a short description of
    their function:
    MaxUploadSpeed - Global maximum upload speed of all transfers
    MaxDownloadSpeed - Global maxium download speed of all transfers
    DefaultDownloadDirectory - Default directory to download files to
    ConfigurationDirectory - Directory to save configuration / download states to
    '''
    properties = {}
    notifiers = {}

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

    def save (self):
        print 'Config.save (): stub'
