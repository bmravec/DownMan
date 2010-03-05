Appdir = $(datadir)/applications
App_in_files = downman.desktop.in
App_DATA = $(App_in_files:.desktop.in=.desktop)

downman.desktop: downman.desktop.in
	@sed -e "s|\@icondir\@|$(pkgdatadir)/imgs|" $< > $@

EXTRA_DIST = downman.desktop.in

CLEANFILES = downman.desktop
