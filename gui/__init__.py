
DEFAULT_GUI = 'gtk'

if DEFAULT_GUI == 'gtk':
    from gtk.application import Application
    from gtk.mainwindow import MainWindow
    from gtk.toolbar import Toolbar
    from gtk.downloadview import DownloadView
    from gtk.stagingview import StagingView
else:
    from application import Application
    from mainwindow import MainWindow
    from toolbar import Toolbar
    from downloadview import DownloadView
    from stagingview import StagingView
