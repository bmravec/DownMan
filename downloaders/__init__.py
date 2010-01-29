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

import hosters

from http import HttpDownload

def create_download (url, downman):
    hoster = hosters.factory.create_host_object (url, downman)

    if hoster != None:
        return hoster

    if url.startswith ('http://'):
        if url.endswith ('.html') or url.endswith ('.php') or url.endswith ('htm'):
            return None
        return HttpDownload (url, downman)

    return None

def load_download (data, downman):
    hoster = hosters.factory.load_host_object (data['match'], downman)

    if hoster != None:
        hoster.startup (data)
        return hoster

    if url.startswith ('http://'):
        if url.endswith ('.html') or url.endswith ('.php') or url.endswith ('htm'):
            return None

        download = HttpDownload (url, downman)
        download.startup (data)
        return download

    return None
