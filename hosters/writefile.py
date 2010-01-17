from threading import Thread
import pycurl

class WriteFile (Thread):
    completed_cb = None
    progress_cb = None

    def __init__ (self, url, filename=None, post_data=None, referer=None):
        Thread.__init__ (self)
        self.url = url
        self.filename = filename
        self.post_data = post_data
        self.referer = referer

    def run (self):
        self.c = pycurl.Curl ()
        self.c.setopt (pycurl.URL, self.url)

        f = open (self.filename, 'wb')
        self.c.setopt (pycurl.WRITEFUNCTION, f.write)

        if self.post_data != None:
            self.c.setopt (pycurl.POSTFIELDS, self.post_data)
            self.c.setopt (pycurl.POST, 1)

        if self.referer != None:
            self.c.setopt (pycurl.REFERER, self.referer)

        if self.progress_cb != None:
            self.c.setopt (pycurl.NOPROGRESS, 0)
            self.c.setopt (pycurl.PROGRESSFUNCTION, self.progress_cb)

        self.c.perform ()
        self.c.close ()
        f.close ()

        if self.completed_cb != None:
            self.completed_cb (self)

    def close (self):
        self.c.close ()
