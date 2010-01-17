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
        c = pycurl.Curl ()
        c.setopt (c.URL, self.url)
        c.setopt (c.WRITEFUNCTION, self.write_cb)

        if self.post_data != None:
            c.setopt (pycurl.POSTFIELDS, self.post_data)
            c.setopt (pycurl.POST, 1)

        if self.referer != None:
            c.setopt (pycurl.REFERER, self.referer)

        if self.headers != None:
            c.setopt (pycurl.HTTPHEADER, self.headers)

        c.perform ()
        c.close ()

        if self.completed_cb != None:
            self.completed_cb (self)

    def write_cb (self, buf):
        self.contents = self.contents + buf
