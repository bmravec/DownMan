#
#       hosterfactory.py
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

import re

class HosterFactory:
    hosters = []

    def add_hoster (self, hoster, mstr):
        self.hosters.append ((hoster, mstr))

    def create_host_object (self, url, downman):
        for hoster in self.hosters:
            if re.match (hoster[1], url) != None:
                return hoster[0] (url, downman)

    def load_host_object (self, match, downman):
        for hoster in self.hosters:
            if hoster[1] == match:
                return hoster[0] (match, downman)
