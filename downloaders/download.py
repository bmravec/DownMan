#
#       download.py
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

STATE_NULL = 0
STATE_PAUSED = 1
STATE_QUEUED = 2
STATE_CONNECTING = 3
STATE_DOWNLOADING = 4
STATE_SEEDING = 5
STATE_WAITING = 6
STATE_HOLDING = 7
STATE_DISABLED = 8
STATE_COMPLETED = 9
STATE_INFO = 10
STATE_INFO_COMPLETED = 11
STATE_NOT_FOUND = 12

class Download:
    links = None
    dmax = -1
    umax = -1
    downloaded = 0
    total = -1
    state = STATE_NULL
    status = ''
    proto = ''
    name = ''

    state_cb = None

    def start_get_info (self, state_cb=None):
        self.state = STATE_INFO
        self.state_cb = state_cb

    def start_download (self, state_cb=None):
        self.state = STATE_CONNECTING
        self.state_cb = state_cb

    def set_state (self, state):
        self.state = state
        if self.state_cb != None:
            self.state_cb (self)

    def startup (self, data):
        '''Setup the download with stored data'''
        pass

    def shutdown (self):
        '''Shuts down the transfer

        Closes all connections and prepares the download to be stored so that it
        can be loaded again at a later running of the application.  This
        function then returns a dictionary of values to be stored in the
        config.xml file.  If the download requires extra binary data, a file in
        the config directory should be created and then linked into the
        entry in the xml with a file entry.
        '''
        print 'Download.shutdown (): stub'
