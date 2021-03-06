#
#       utils.py
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

def size_to_string (size):
    digits = 2

    if size < 1024:
        return '%d Bytes' % (size)

    size = size / 1024.0

    if size < 1024:
        if size > 100:
            digits = 1
        return '%.*f KB' % (digits, size)

    size = size / 1024.0

    if size < 1024:
        if size > 100:
            digits = 1
        return '%.*f MB' % (digits, size)

    size = size / 1024.0

    if size > 1000:
        digits = 0
    elif size > 100:
        digits = 1

    return '%.*f GB' % (digits, size)
