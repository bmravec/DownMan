import gtk

class DownloadView (gtk.TreeView):
    def __init__ (self, app):
        gtk.TreeView.__init__ (self)

        self.app = app

        self.store = gtk.TreeStore (object, str, str, int, int)
        self.set_model (self.store)

        self.get_selection ().set_mode (gtk.SELECTION_MULTIPLE)

        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Name', cell, text=1)
        column.set_expand (True)
        self.append_column (column)
        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Status', cell, text=2)
        column.set_expand (True)
        self.append_column (column)
        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Downloaded', cell, text=3)
        self.append_column (column)
        cell = gtk.CellRendererText ()
        column = gtk.TreeViewColumn ('Total', cell, text=4)
        self.append_column (column)

    def add_download (self, download):
        iter = self.store.append (None, row=(download, download.name, '', download.downloaded, download.total))

    def update_download (self, download):
        for d in self.store:
            if d[0] == download:
                d[1] = download.name
                d[2] = download.status
                d[3] = download.downloaded
                d[4] = download.total

    def remove_download (self, download):
        for d in self.store:
            if d[0] == download:
                self.store.remove (d.iter)
                return

    def get_selected (self):
        ds = []

        model, rows = self.get_selection ().get_selected_rows ()

        for row in rows:
            ds.append (model[row][0])

        return ds
