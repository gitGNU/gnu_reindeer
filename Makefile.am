SUBDIRS = libgnu
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = \
    -I$(top_builddir) -I$(top_srcdir) $(LTDLINCL) \
    $(GLIB_CFLAGS) \
    -D AC_libdir=\"$(libdir)\"
AM_LDFLAGS =
AM_XSLTPROC_FLAGS = \
    --xinclude \
    --path /usr/share/xml/docbook/stylesheet/docbook-xsl

BUILT_SOURCES =
EXTRA_DIST =
CLEANFILES =
MOSTLYCLEANFILES =

include_HEADERS =
noinst_LTLIBRARIES =
lib_LTLIBRARIES =
EXTRA_LTLIBRARIES =

renlibdir = $(libdir)/reindeer-0.0
renlib_LTLIBRARIES =

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

#
# libreindeer-debug
#

renlib_LTLIBRARIES += debug/libreindeer-debug.la

debug_libreindeer_debug_la_SOURCES = \
    debug/debug.c

debug_libreindeer_debug_la_LIBADD = \
    $(LTLIBOBJS) reindeer/libreindeer-0.0.la

debug_libreindeer_debug_la_LDFLAGS = \
    -module -version-info 0:0:0

#
# documentation
#

reindeer_DOCBOOK = \
    doc/reindeer.docbook \
    doc/about.docbook \
    doc/obtaining.docbook \
    doc/compiling.docbook \
    doc/basic-example.docbook \
    doc/thread-safety.docbook \
    doc/types.docbook \
    doc/init-fini.docbook \
    doc/colors.docbook \
    doc/data-blocks.docbook \
    doc/matrices.docbook \
    doc/vectors.docbook \
    doc/buffers.docbook \
    doc/transformation.docbook \
    doc/coord-arrays.docbook \
    doc/color-arrays.docbook \
    doc/normal-arrays.docbook \
    doc/lights.docbook \
    doc/materials.docbook \
    doc/templates.docbook \
    doc/objects.docbook \
    doc/fdl-1.3.docbook

CLEANFILES += \
    doc/reindeer.html doc/html_node.stamp doc/html_node/*.html
EXTRA_DIST += \
    $(reindeer_DOCBOOK) \
    doc/common.xsl doc/html.xsl doc/html-chunk.xsl

if HAVE_DOCBOOK_XSLT
dist_html_DATA = doc/reindeer.html

htmlnodedir = $(htmldir)/html_node
dist_htmlnode_DATA = doc/html_node/*.html
endif

doc/reindeer.html: $(reindeer_DOCBOOK) $(srcdir)/doc/html.xsl
	$(XSLTPROC) -o $@ $(AM_XSLTPROC_FLAGS) $(XSLTPROC_FLAGS) \
		--path /usr/share/xml/docbook/stylesheet/docbook-xsl/html \
		$(srcdir)/doc/html.xsl $(srcdir)/doc/reindeer.docbook

doc/html_node/*.html: doc/html_node.stamp

doc/html_node.stamp: $(reindeer_DOCBOOK) $(srcdir)/doc/html-chunk.xsl
	$(XSLTPROC) -o doc/html_node/ $(AM_XSLTPROC_FLAGS) $(XSLTPROC_FLAGS) \
		--path /usr/share/xml/docbook/stylesheet/docbook-xsl/html \
		$(srcdir)/doc/html-chunk.xsl $(srcdir)/doc/reindeer.docbook
	touch $@
