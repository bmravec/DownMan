INCLUDES = $(GTK_CFLAGS) $(GLIB_CFLAGS) $(SIGC_CFLAGS) $(CURL_CFLAGS) \
    -DSHARE_DIR=\"$(pkgdatadir)\"

gtksources = \
    gtk/application-gtk.cc gtk/application-gtk.h   \
    gtk/mainwindow-gtk.cc gtk/mainwindow-gtk.h     \
    gtk/downloadview-gtk.cc gtk/downloadview-gtk.h \
    gtk/toolbar-gtk.cc gtk/toolbar-gtk.h

guisources = \
    gui-factory.cc gui-factory.h   \
    application.cc application.h   \
    mainwindow.cc mainwindow.h     \
    downloadview.cc downloadview.h \
    stagingview.cc stagingview.h   \
    toolbar.cc toolbar.h

bin_PROGRAMS = downman

downman_LDADD = $(GTK_LIBS) $(GLIB_LIBS) $(SIGC_LIBS) $(CURL_LIBS)

downman_SOURCES =                  \
    downman.cc downman.h           \
    dregex.cc dregex.h             \
    staginglist.cc staginglist.h   \
    download.cc download.h         \
    http-download.cc http-download.h \
    $(guisources) $(gtksources)

EXTRA_DIST=
CLEANFILES=
