import gtk
import time
import gobject

class MainWindow (gtk.Window):
    prev_progress = None
    prev_downloaded = None

    def __init__ (self, downman):
        gtk.Window.__init__ (self, gtk.WINDOW_TOPLEVEL)

        self.downman = downman

        self.set_default_size (400, 300)
        self.connect ("destroy", self.destroy)

        self.vbox = gtk.VBox ()
        self.add (self.vbox)

        self.show_all ()

    def destroy (self, widget):
        self.downman.app.quit ()

    def pack_widget (self, widget):
        self.vbox.pack_start (widget)
