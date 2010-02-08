/*
 *      toolbar-gtk.cc
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

#include <stdio.h>

#include "downman.h"
#include "toolbar.h"
#include "toolbar-gtk.h"

ToolbarGtk::ToolbarGtk (DownMan *downman) : Toolbar (downman)
{
    widget = gtk_toolbar_new ();

    add_url = gtk_tool_button_new_from_stock (GTK_STOCK_NETWORK);
    gtk_tool_item_set_tooltip_text (add_url, "Add URL");
    g_signal_connect (add_url, "clicked", G_CALLBACK (ToolbarGtk::add_url_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), add_url, -1);

    add_file = gtk_tool_button_new_from_stock (GTK_STOCK_OPEN);
    gtk_tool_item_set_tooltip_text (add_file, "Add File");
    g_signal_connect (add_file, "clicked", G_CALLBACK (ToolbarGtk::add_file_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), add_file, -1);

    remove = gtk_tool_button_new_from_stock (GTK_STOCK_REMOVE);
    gtk_tool_item_set_tooltip_text (remove, "Remove Selected");
    g_signal_connect (remove, "clicked", G_CALLBACK (ToolbarGtk::remove_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), remove, -1);

    gtk_toolbar_insert (GTK_TOOLBAR (widget), gtk_separator_tool_item_new (), -1);

    start = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
    gtk_tool_item_set_tooltip_text (start, "Start");
    g_signal_connect (start, "clicked", G_CALLBACK (ToolbarGtk::start_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), start, -1);

    stop = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_STOP);
    gtk_tool_item_set_tooltip_text (stop, "Remove Selected");
    g_signal_connect (stop, "clicked", G_CALLBACK (ToolbarGtk::stop_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), stop, -1);

    gtk_toolbar_insert (GTK_TOOLBAR (widget), gtk_separator_tool_item_new (), -1);

    start_staging = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_NEXT);
    gtk_tool_item_set_tooltip_text (start_staging, "Start Staging");
    g_signal_connect (start_staging, "clicked", G_CALLBACK (ToolbarGtk::start_staging_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), start_staging, -1);

    clear_staging = gtk_tool_button_new_from_stock (GTK_STOCK_CLEAR);
    gtk_tool_item_set_tooltip_text (clear_staging, "Remove Selected");
    g_signal_connect (clear_staging, "clicked", G_CALLBACK (ToolbarGtk::clear_staging_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), clear_staging, -1);

    gtk_widget_show_all (widget);
}

ToolbarGtk::~ToolbarGtk ()
{

}

void
ToolbarGtk::set_start_enabled (bool enabled)
{
    printf ("ToolbarGtk::set_start_enabled (enabled): stub\n");
}

void
ToolbarGtk::set_stop_enabled (bool enabled)
{
    printf ("ToolbarGtk::set_stop_enabled (enabled): stub\n");
}

GtkWidget*
ToolbarGtk::get_widget ()
{
    return widget;
}

void
ToolbarGtk::add_url_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    tb->downman->prompt_for_urls ();
}

void
ToolbarGtk::add_file_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    printf ("ToolbarGtk::add_file_cb (): stub\n");
}

void
ToolbarGtk::remove_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    printf ("ToolbarGtk::remove_cb(): stub\n");
}

void
ToolbarGtk::start_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    printf ("ToolbarGtk::start_cb (): stub\n");
}

void
ToolbarGtk::stop_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    printf ("ToolbarGtk::stop_cb (): stub\n");
}

void
ToolbarGtk::start_staging_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    printf ("ToolbarGtk::start_staging_cb (): stub\n");
}

void
ToolbarGtk::clear_staging_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    printf ("ToolbarGtk::clear_staging_cb (): stub\n");
}
