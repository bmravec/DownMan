INCLUDES = $(GTK_CFLAGS) $(GLIB_CFLAGS)

gtksources = \
    gtk/gtk-application.cc gtk/gtk-application.h \
    gtk/gtk-mainwindow.cc gtk/gtk-mainwindow.h

bin_PROGRAMS = downman

downman_LDADD = $(GTK_LIBS) $(GLIB_LIBS)

downman_SOURCES =                \
    downman.cc downman.h         \
    application.cc application.h \
    mainwindow.cc mainwindow.h   \
    gui-factory.cc gui-factory.h \
    $(gtksources)

EXTRA_DIST=
CLEANFILES=
