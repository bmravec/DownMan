import gtk

from mainwindow import MainWindow
from downloadview import DownloadView

gtk.gdk.threads_init ()

class Application:
    def __init__ (self, downman):
        self.downman = downman

        self.win = MainWindow (self.downman)
        self.dv = DownloadView (self.downman)

        scrolled = gtk.ScrolledWindow ()
        scrolled.add (self.dv)
        scrolled.set_policy (gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC)
        scrolled.show_all ()
        self.win.pack_widget (scrolled)

    def run (self):
        gtk.main ()

    def quit (self):
        gtk.main_quit ()

    def prompt_for_captcha (self, imagedata):
        code = None

        gtk.gdk.threads_enter ()

        pbloader = gtk.gdk.PixbufLoader ()
        pbloader.write (imagedata)
        pbloader.close ()

        pixbuf = pbloader.get_pixbuf ()

        img = gtk.image_new_from_pixbuf (pixbuf)

        entry = gtk.Entry ()

        dialog = gtk.Dialog ('Enter Captcha Code...',
            buttons=('Cancel', gtk.RESPONSE_CANCEL, 'Ok', gtk.RESPONSE_OK))

        entry.connect ('activate', self.prompt_entry_activated, dialog)

        ca = dialog.get_content_area ()
        ca.pack_start (img)
        ca.pack_start (entry)
        ca.show_all ()

        rid = dialog.run ()

        if rid == gtk.RESPONSE_OK:
            code = entry.get_text ()

        dialog.destroy ()

        gtk.gdk.threads_leave ()

        return code

    def prompt_entry_activated (self, entry, dialog):
        dialog.response (gtk.RESPONSE_OK)

    def add_download (self, download):
        self.dv.add_download (download)

    def update_download (self, download):
        self.dv.update_download (download)

    def remove_download (self, download):
        self.dv.remove_download (download)
