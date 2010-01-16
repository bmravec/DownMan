import re

class HosterFactory:
    def __init__ (self):
        self.hosters = []

    def add_hoster (self, hoster, mstr):
        self.hosters.append ((hoster, mstr))

    def create_host_object (self, url, app):
        for hoster in self.hosters:
            if re.match (hoster[1], url) != None:
                return hoster[0] (url, app)
