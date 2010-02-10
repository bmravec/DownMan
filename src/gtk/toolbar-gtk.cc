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

#include <iostream>

#include "toolbar-gtk.h"

ToolbarGtk::ToolbarGtk () : Toolbar ()
{
    widget = gtk_toolbar_new ();

    add_url_item = gtk_tool_button_new_from_stock (GTK_STOCK_NETWORK);
    gtk_tool_item_set_tooltip_text (add_url_item, "Add URL");
    g_signal_connect (add_url_item, "clicked",
        G_CALLBACK (ToolbarGtk::add_url_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), add_url_item, -1);

    add_file_item = gtk_tool_button_new_from_stock (GTK_STOCK_OPEN);
    gtk_tool_item_set_tooltip_text (add_file_item, "Add File");
    g_signal_connect (add_file_item, "clicked",
        G_CALLBACK (ToolbarGtk::add_file_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), add_file_item, -1);

    remove_item = gtk_tool_button_new_from_stock (GTK_STOCK_REMOVE);
    gtk_tool_item_set_tooltip_text (remove_item, "Remove Selected");
    g_signal_connect (remove_item, "clicked",
        G_CALLBACK (ToolbarGtk::remove_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), remove_item, -1);

    gtk_toolbar_insert (GTK_TOOLBAR (widget), gtk_separator_tool_item_new (), -1);

    start_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
    gtk_tool_item_set_tooltip_text (start_item, "Start");
    g_signal_connect (start_item, "clicked",
        G_CALLBACK (ToolbarGtk::start_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), start_item, -1);

    stop_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_STOP);
    gtk_tool_item_set_tooltip_text (stop_item, "Remove Selected");
    g_signal_connect (stop_item, "clicked",
        G_CALLBACK (ToolbarGtk::stop_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), stop_item, -1);

    gtk_toolbar_insert (GTK_TOOLBAR (widget), gtk_separator_tool_item_new (), -1);

    start_staging_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_NEXT);
    gtk_tool_item_set_tooltip_text (start_staging_item, "Start Staging");
    g_signal_connect (start_staging_item, "clicked",
        G_CALLBACK (ToolbarGtk::start_staging_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), start_staging_item, -1);

    clear_staging_item = gtk_tool_button_new_from_stock (GTK_STOCK_CLEAR);
    gtk_tool_item_set_tooltip_text (clear_staging_item, "Remove Selected");
    g_signal_connect (clear_staging_item, "clicked",
        G_CALLBACK (ToolbarGtk::clear_staging_cb), this);
    gtk_toolbar_insert (GTK_TOOLBAR (widget), clear_staging_item, -1);

    gtk_widget_show_all (widget);
}

ToolbarGtk::~ToolbarGtk ()
{

}

void
ToolbarGtk::set_start_enabled (bool enabled)
{
    std::cout << "ToolbarGtk::set_start_enabled (enabled): stub\n";
}

void
ToolbarGtk::set_stop_enabled (bool enabled)
{
    std::cout << "ToolbarGtk::set_stop_enabled (enabled): stub\n";
}

GtkWidget*
ToolbarGtk::get_widget ()
{
    return widget;
}

void
ToolbarGtk::add_url_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    tb->add_url ();
}

void
ToolbarGtk::add_file_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    tb->add_file ();
}

void
ToolbarGtk::remove_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    tb->remove ();
}

void
ToolbarGtk::start_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    tb->start ();
}

void
ToolbarGtk::stop_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    tb->stop ();
}

void
ToolbarGtk::start_staging_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    tb->start_staging ();
}

void
ToolbarGtk::clear_staging_cb (GtkWidget *widget, ToolbarGtk *tb)
{
    tb->clear_staging ();
}
