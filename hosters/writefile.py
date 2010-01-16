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
        c = pycurl.Curl ()
        c.setopt (c.URL, self.url)

        f = open (self.filename, 'wb')
        c.setopt (c.WRITEFUNCTION, f.write)

        if self.post_data != None:
            c.setopt (pycurl.POSTFIELDS, self.post_data)
            c.setopt (pycurl.POST, 1)

        if self.referer != None:
            c.setopt (pycurl.REFERER, self.referer)

        if self.progress_cb != None:
            c.setopt (pycurl.NOPROGRESS, 0)
            c.setopt (pycurl.PROGRESSFUNCTION, self.progress_cb)

        c.perform ()
        c.close ()
        f.close ()

        if self.completed_cb != None:
            self.completed_cb (self)

