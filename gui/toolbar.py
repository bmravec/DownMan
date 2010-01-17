import gtk

class Toolbar (gtk.Toolbar):
    def __init__ (self, app):
        gtk.Toolbar.__init__ (self)

        self.app = app

        self.add_button = gtk.ToolButton (gtk.STOCK_ADD)
        self.add_button.set_tooltip_text ('Add URL')
        self.add_button.connect ('clicked', self.add_clicked)
        self.insert (self.add_button, -1)

        self.remove_button = gtk.ToolButton (gtk.STOCK_REMOVE)
        self.remove_button.set_tooltip_text ('Remove Selected')
        self.remove_button.connect ('clicked', self.remove_clicked)
        self.insert (self.remove_button, -1)

        sep = gtk.SeparatorToolItem ()
        self.insert (sep, -1)

        self.start_button = gtk.ToolButton (gtk.STOCK_MEDIA_PLAY)
        self.start_button.set_tooltip_text ('Start')
        self.start_button.connect ('clicked', self.start_clicked)
        self.insert (self.start_button, -1)

        self.pause_button = gtk.ToolButton (gtk.STOCK_MEDIA_PAUSE)
        self.pause_button.set_tooltip_text ('Pause')
        self.pause_button.connect ('clicked', self.pause_clicked)
        self.insert (self.pause_button, -1)

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

    def remove_clicked (self, item):
        ds = self.app.dv.get_selected ()
        for d in ds:
            self.app.downman.remove_download (d)

    def start_clicked (self, item):
        print 'Start Clicked'

    def pause_clicked (self, item):
        print 'Pause Clicked'
