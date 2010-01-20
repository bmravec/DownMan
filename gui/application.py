#
#       application.py
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

class Application:
    """Main gui class.

    The Application class handles all basic gui operation.  It handles the
    application loop and quiting that, and also provides any extra functions
    that do not fit into any of the other gui classes
    """
    def __init__ (self, downman):
        self.downman = downman

    def run (self):
        """Start main application loop.
        """
        print 'Application.run (): stub'

    def quit (self):
        """Stop main application loop.
        """
        print 'Application.quit (): stub'

    def prompt_for_captcha (self, imagedata):
        """Prompt the user for a captcha string

        Takes the raw data of an image as a parameter
        The application class will display that image to the user and prompt
        them to enter the code.  The application class will then return either
        the entered code when the user hit ok, or None when the user canceled
        the operation.
        """
        print 'Application.prompt_for_captcha (imagedata): stub'

        return None
