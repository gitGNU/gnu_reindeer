SUBDIRS = libgnu libltdl ren reindeer
ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = m4/gnulib-cache.m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = reindeer.pc

dist-hook: gen-ChangeLog

.PHONY: gen-ChangeLog
gen-ChangeLog:
	if test -d $(top_srcdir)/.git; then \
		(cd $(top_srcdir) && ./build-aux/gitlog-to-changelog) > $(distdir)/cl-t; \
		rm -f $(distdir)/ChangeLog; \
		mv $(distdir)/cl-t $(distdir)/ChangeLog; \
	fi

