/*
 *      toolbar.h
 *
 *      Copyright 2010 Brett Mravec <brett.mravec@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

class Toolbar;

#ifndef __TOOLBAR_H__
#define __TOOLBAR_H__

#include <sigc++/sigc++.h>

class Toolbar {
    public:
        Toolbar ();
        ~Toolbar ();

        virtual void set_start_enabled (bool enabled);
        virtual void set_stop_enabled (bool enabled);

        sigc::signal<void> &signal_add_url () { return add_url; }
        sigc::signal<void> &signal_add_file () { return add_file; }
        sigc::signal<void> &signal_remove () { return remove; }
        sigc::signal<void> &signal_start () { return start; }
        sigc::signal<void> &signal_stop () { return stop; }
        sigc::signal<void> &signal_start_staging () { return start_staging; }
        sigc::signal<void> &signal_clear_staging () { return clear_staging; }

    protected:
        sigc::signal<void> add_url;
        sigc::signal<void> add_file;
        sigc::signal<void> remove;
        sigc::signal<void> start;
        sigc::signal<void> stop;
        sigc::signal<void> start_staging;
        sigc::signal<void> clear_staging;

    private:
};

#endif /* __TOOLBAR_H__ */
