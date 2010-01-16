import gtk

class Toolbar (gtk.Toolbar):
    def __init__ (self, app):
        gtk.Toolbar.__init__ (self)

        self.app = app

        self.add_button = gtk.ToolButton (gtk.STOCK_ADD)
        self.add_button.set_tooltip_text ('Add URL')
        self.add_button.connect ('clicked', self.add_clicked)
        self.insert (self.add_button, -1)

    def add_clicked (self, item):
        textview = gtk.TextView ()

        dialog = gtk.Dialog ('Add URLs...',
            buttons=('Cancel', gtk.RESPONSE_CANCEL, 'Ok', gtk.RESPONSE_OK))

        ca = dialog.get_content_area ()
        ca.pack_start (gtk.Label ("Enter URLs below:"), expand=False)

        sw = gtk.ScrolledWindow ()
        sw.set_policy (gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
        sw.add (textview)
        ca.pack_start (sw)
        ca.show_all ()

        rid = dialog.run ()

        if rid == gtk.RESPONSE_OK:
            textbuf = textview.get_buffer ()
            text = textbuf.get_text (textbuf.get_start_iter (), textbuf.get_end_iter ())
            self.app.downman.parse_download (text)

        dialog.destroy ()
