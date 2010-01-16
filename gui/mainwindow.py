import gtk
import time
import gobject

class MainWindow:
    prev_progress = None
    prev_downloaded = None

    def __init__ (self, downman):
        self.downman = downman

        self.window = gtk.Window (gtk.WINDOW_TOPLEVEL)
        self.window.set_default_size (400, 300)

        self.window.connect ("destroy", self.destroy)

        vbox = gtk.VBox ()

        self.store = gtk.TreeStore (object, str, str, int, int)
        self.view = gtk.TreeView (self.store)

        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Name', cell, text=1)
        column.set_expand (True)
        self.view.append_column (column)
        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Status', cell, text=2)
        column.set_expand (True)
        self.view.append_column (column)
        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Downloaded', cell, text=3)
        self.view.append_column (column)
        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Total', cell, text=4)
        self.view.append_column (column)

        scrolled = gtk.ScrolledWindow ()
        scrolled.add (self.view)

        vbox.pack_start (scrolled)

        self.window.add (vbox)

        self.window.show_all ()

    def add_download (self, download):
        iter = self.store.append (None, row=(download, '', '', 0, 0))

    def update_download (self, download):
        for d in self.store:
            if d[0] == download:
                d[1] = download.url
                d[2] = download.status
                d[3] = download.downloaded
                d[4] = download.total

    def remove_download (self, download):
        for d in self.store:
            if d[0] == download:
                self.store.remove (d)
                return

    def destroy (self, widget):
        gtk.main_quit ()

    def download_progress (self, dt, dd, ut, ud):
        if self.prev_progress == None:
            self.prev_progress = time.time ()
            self.prev_downloaded = dd
        else:
            ctime = time.time ()
            speed = (dd - self.prev_downloaded) / (ctime - self.prev_progress)

            self.prev_progress = ctime
            self.prev_downloaded = dd

            gtk.gdk.threads_enter ()
            self.percent.set_text ('%d / %d' % (dd, dt))
            self.speed.set_text ('%d Bps' % (speed))
            gtk.gdk.threads_leave ()

    def update_progress (self, dt, dd):
        self.percent.set_text ('%d / %d' % (dd, dt))

    def update_speed (self, speed):
        self.speed.set_text ('%d Bps' % (speed))
