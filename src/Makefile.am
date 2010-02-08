INCLUDES = $(GTK_CFLAGS) $(GLIB_CFLAGS) -DSHARE_DIR=\"$(pkgdatadir)\"

gtksources = \
    gtk/gtk-application.cc gtk/gtk-application.h   \
    gtk/gtk-mainwindow.cc gtk/gtk-mainwindow.h     \
    gtk/gtk-downloadview.cc gtk/gtk-downloadview.h 

bin_PROGRAMS = downman

downman_LDADD = $(GTK_LIBS) $(GLIB_LIBS)

downman_SOURCES =                  \
    downman.cc downman.h           \
    application.cc application.h   \
    mainwindow.cc mainwindow.h     \
    downloadview.cc downloadview.h \
    gui-factory.cc gui-factory.h   \
    $(gtksources)

EXTRA_DIST=
CLEANFILES=
