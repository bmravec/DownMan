/*
 *      application.cc
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

#include "application.h"
#include "downman.h"

Application::Application (DownMan *downman)
{
    this->downman = downman;
}

Application::~Application ()
{

}

void
Application::run ()
{
    printf ("Application::run (): stub\n");
}

void
Application::quit ()
{
    printf ("Application::quit (): stub\n");
}

char*
Application::prompt_for_captcha (char *img_data)
{
    printf ("Application::prompt_for_captcha (): stub\n");

    return NULL;
}

char*
Application::prompt_for_urls ()
{
    printf ("Application::prompt_for_urls (): stub\n");

    return NULL;
}

char*
Application::prompt_for_files ()
{
    printf ("Application::prompt_for_files (): stub\n");

    return NULL;
}
