SUBDIRS = libgnu
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = \
    -I$(top_builddir) -I$(top_srcdir) $(LTDLINCL) \
    $(GLIB_CFLAGS) \
    -D AC_libdir=\"$(libdir)\"
AM_LDFLAGS =

BUILT_SOURCES =
EXTRA_DIST =
CLEANFILES =
MOSTLYCLEANFILES =

include_HEADERS =
noinst_LTLIBRARIES =
lib_LTLIBRARIES =
EXTRA_LTLIBRARIES =

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA =

EXTRA_DIST += m4/gnulib-cache.m4

dist-hook: gen-ChangeLog

.PHONY: gen-ChangeLog
gen-ChangeLog:
	if test -d $(top_srcdir)/.git; then \
		(cd $(top_srcdir) && ./build-aux/gitlog-to-changelog) > $(distdir)/cl-t; \
		rm -f $(distdir)/ChangeLog; \
		mv $(distdir)/cl-t $(distdir)/ChangeLog; \
	fi

#
# libltdl
#

include libltdl/Makefile.inc

#
# libreindeer
#

lib_LTLIBRARIES += reindeer/libreindeer-0.0.la

reindeer_libreindeer_0_0_la_SOURCES = \
    reindeer/reindeer.c reindeer/reindeer.h \
    reindeer/backdata.c reindeer/backdata.h \
    reindeer/matrix.c reindeer/vector.c reindeer/color.c reindeer/datablock.c \
    reindeer/coordarray.c reindeer/colorarray.c reindeer/normalarray.c \
    reindeer/indexarray.c \
    reindeer/material.c reindeer/light.c \
    reindeer/template.c reindeer/template.h \
    reindeer/object.c reindeer/object.h

reindeer_libreindeer_0_0_la_LIBADD = \
    $(LTLIBOBJS) $(LIBLTDL) $(GLIB_LIBS)

reindeer_libreindeer_0_0_la_LDFLAGS = \
    -export-dynamic -version-info 0:0:0

renincludedir = $(includedir)/reindeer-0.0/ren
reninclude_HEADERS = \
    ren/ren.h ren/types.h ren/base.h ren/impl.h ren/tmpl.h ren/funcs.h \
        ren/matrix.h ren/vector.h ren/color.h ren/datablock.h \
        ren/coordarray.h ren/colorarray.h ren/normalarray.h \
        ren/indexarray.h \
        ren/material.h ren/light.h \
        ren/template.h ren/object.h

pkgconfig_DATA += reindeer/reindeer-0.0.pc
