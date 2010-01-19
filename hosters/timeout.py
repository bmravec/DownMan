#
#       timeout.py
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

from threading import Thread
import time

class Timeout (Thread):
    def __init__ (self, dur, done_cb, progress_cb=None):
        Thread.__init__ (self)
        self.progress_cb = progress_cb
        self.done_cb = done_cb
        self.dur = dur

        self.start ()

    def run (self):
        while self.dur > 0:
            time.sleep (1)
            self.dur = self.dur - 1
            if self.progress_cb != None:
                self.progress_cb (self.dur)
        self.done_cb ()
