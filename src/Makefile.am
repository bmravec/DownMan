INCLUDES = $(GTK_CFLAGS) $(GLIB_CFLAGS) $(SIGC_CFLAGS) $(LIBXML_CFLAGS) \
    -DSHARE_DIR=\"$(pkgdatadir)\"

gtksources = \
    gtk/application-gtk.cc gtk/application-gtk.h   \
    gtk/mainwindow-gtk.cc gtk/mainwindow-gtk.h     \
    gtk/downloadview-gtk.cc gtk/downloadview-gtk.h \
    gtk/stagingview-gtk.cc gtk/stagingview-gtk.h   \
    gtk/toolbar-gtk.cc gtk/toolbar-gtk.h

guisources = \
    gui-factory.cc gui-factory.h   \
    application.cc application.h   \
    mainwindow.cc mainwindow.h     \
    downloadview.cc downloadview.h \
    stagingview.cc stagingview.h   \
    toolbar.cc toolbar.h

bin_PROGRAMS = downman

downman_LDADD = $(GTK_LIBS) $(GLIB_LIBS) $(SIGC_LIBS) $(LIBXML_LIBS)

downman_SOURCES =                          \
    downman.cc downman.h                   \
    app-config.cc app-config.h             \
    app-log.cc app-log.h                   \
    speed-monitor.cc speed-monitor.h       \
    speed-object.cc speed-object.h         \
    utils.cc utils.h                       \
    dregex.cc dregex.h                     \
    url.cc url.h                           \
    socket.cc socket.h                     \
    http-connection.cc http-connection.h   \
    ftp-connection.cc ftp-connection.h     \
    dlist.cc dlist.h                       \
    downloadlist.cc downloadlist.h         \
    staginglist.cc staginglist.h           \
    download-factory.cc download-factory.h \
    download.cc download.h                 \
    http-download.cc http-download.h       \
    ftp-download.cc ftp-download.h         \
    $(guisources) $(gtksources)

EXTRA_DIST=
CLEANFILES=
