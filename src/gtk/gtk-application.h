/*
 *      gtk-application.c
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

#ifndef __GTK_APPLICATION_H__
#define __GTK_APPLICATION_H__

#include "application.h"

class DownMan;

class GtkApplication : public Application {
    public:
        GtkApplication (DownMan *downman);
        ~GtkApplication ();

        void run ();
        void quit ();

        char *prompt_for_captcha (char *img_data);
        char *prompt_for_urls ();
        char *prompt_for_files ();
    protected:
        DownMan *downman;
};

#endif /* __GTK_APPLICATION_H__ */
