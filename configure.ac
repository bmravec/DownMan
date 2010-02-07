AC_PREREQ(2.59)
AC_INIT([DownMan],[0.1])
AC_CONFIG_SRCDIR([src/downman.cc])
AM_INIT_AUTOMAKE([dist-bzip2])

AC_CONFIG_HEADERS(config.h)

AC_PROG_CC
AC_PROG_CXX

AC_PROG_INSTALL
AC_PROG_LIBTOOL

AM_PATH_GLIB_2_0(,,,gthread gobject gio)
AM_PATH_GTK_2_0()

AC_OUTPUT(
Makefile
src/Makefile
images/Makefile
)
