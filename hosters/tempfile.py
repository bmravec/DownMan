from threading import Thread
import pycurl

class TempFile (Thread):
    completed_cb = None

    def __init__ (self, url, post_data=None, referer=None, headers=None):
        Thread.__init__ (self)
        self.url = url
        self.post_data = post_data
        self.referer = referer
        self.contents = ''
        self.headers = headers

    def run (self):
        self.c = pycurl.Curl ()
        self.c.setopt (pycurl.URL, self.url)
        self.c.setopt (pycurl.WRITEFUNCTION, self.write_cb)

        if self.post_data != None:
            self.c.setopt (pycurl.POSTFIELDS, self.post_data)
            self.c.setopt (pycurl.POST, 1)

        if self.referer != None:
            self.c.setopt (pycurl.REFERER, self.referer)

        if self.headers != None:
            self.c.setopt (pycurl.HTTPHEADER, self.headers)

        self.c.perform ()
        self.c.close ()

        if self.completed_cb != None:
            self.completed_cb (self)

    def write_cb (self, buf):
        self.contents = self.contents + buf

    def close (self):
        self.c.close ()
