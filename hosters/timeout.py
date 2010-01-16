from threading import Thread
import time

class Timeout (Thread):
    def __init__ (self, dur, done_cb, progress_cb=None):
        Thread.__init__ (self)
        self.progress_cb = progress_cb
        self.done_cb = done_cb
        self.dur = dur

        self.start ()

    def run (self):
        while self.dur > 0:
            time.sleep (1)
            self.dur = self.dur - 1
            if self.progress_cb != None:
                self.progress_cb (self.dur)
        self.done_cb ()
