/*
 *      toolbar-gtk.h
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

class ToolbarGtk;

#ifndef __TOOLBAR_GTK_H__
#define __TOOLBAR_GTK_H__

#include <gtk/gtk.h>

#include "toolbar.h"

class ToolbarGtk : public Toolbar {
    public:
        ToolbarGtk ();
        ~ToolbarGtk ();

        void set_start_enabled (bool enabled);
        void set_stop_enabled (bool enabled);

        GtkWidget *get_widget ();

    private:
        GtkWidget *widget;

        GtkToolItem *add_url_item;
        GtkToolItem *add_file_item;
        GtkToolItem *remove_item;
        GtkToolItem *start_item;
        GtkToolItem *stop_item;
        GtkToolItem *start_staging_item;
        GtkToolItem *clear_staging_item;

        static void add_url_cb (GtkWidget *widget, ToolbarGtk *tb);
        static void add_file_cb (GtkWidget *widget, ToolbarGtk *tb);
        static void remove_cb (GtkWidget *widget, ToolbarGtk *tb);
        static void start_cb (GtkWidget *widget, ToolbarGtk *tb);
        static void stop_cb (GtkWidget *widget, ToolbarGtk *tb);
        static void start_staging_cb (GtkWidget *widget, ToolbarGtk *tb);
        static void clear_staging_cb (GtkWidget *widget, ToolbarGtk *tb);
};

#endif /* __TOOLBAR_GTK_H__ */
