
DEFAULT_GUI = 'gtk'

if DEFAULT_GUI == 'gtk':
    from gtk.application import Application
    from gtk.downloadview import DownloadView
    from gtk.mainwindow import MainWindow
    from gtk.toolbar import Toolbar
else:
    from application import Application
    from downloadview import DownloadView
    from mainwindow import MainWindow
    from toolbar import Toolbar
