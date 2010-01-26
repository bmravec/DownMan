
DEFAULT_GUI = 'gtk'

if DEFAULT_GUI == 'gtk':
    from gtk.application import Application
    from gtk.mainwindow import MainWindow
    from gtk.toolbar import Toolbar
    from gtk.downloadview import DownloadView
    from gtk.stagingview import StagingView
    from gtk.config import Config
elif DEFAULT_GUI == 'qt':
    from qt.application import Application
    from qt.mainwindow import MainWindow
    from qt.toolbar import Toolbar
    from qt.downloadview import DownloadView
    from qt.stagingview import StagingView
    from qt.config import Config
else:
    from application import Application
    from mainwindow import MainWindow
    from toolbar import Toolbar
    from downloadview import DownloadView
    from stagingview import StagingView
    from config import Config
