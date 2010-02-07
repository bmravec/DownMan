/*
 *      gtk-application.cc
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

#include <gtk/gtk.h>

#include "gtk-application.h"
#include "downman.h"

GtkApplication::GtkApplication (DownMan *downman) : Application (downman)
{
    g_type_init ();

    g_thread_init (NULL);
    gdk_threads_init ();

    gint argc = 0;
    gtk_init (&argc, NULL);
}

GtkApplication::~GtkApplication ()
{

}

void
GtkApplication::run ()
{
    gdk_threads_enter ();
    gtk_main ();
    gdk_threads_leave ();
}

void
GtkApplication::quit ()
{
    gtk_main_quit ();
}

char*
GtkApplication::prompt_for_captcha (char *img_data)
{
    printf ("GtkApplication::prompt_for_captcha (): stub\n");

    return NULL;
}

char*
GtkApplication::prompt_for_urls ()
{
    printf ("GtkApplication::prompt_for_urls (): stub\n");

    return NULL;
}

char*
GtkApplication::prompt_for_files ()
{
    printf ("GtkApplication::prompt_for_files (): stub\n");

    return NULL;
}
