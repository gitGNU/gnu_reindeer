lib_LTLIBRARIES = libreindeer.la

libreindeer_la_SOURCES = \
    reindeer.c reindeer.h backdata.c backdata.h \
    matrix.c vector.c color.c datablock.c \
    coordarray.c colorarray.c normalarray.c \
    indexarray.c \
    template.c template.h object.c object.h \
    light.c material.c

libreindeer_la_CPPFLAGS = \
    -I$(top_builddir) -I$(top_srcdir) $(LTDLINCL) \
    -D AC_libdir=\"$(libdir)\"

libreindeer_la_CFLAGS = $(GLIB_CFLAGS)

libreindeer_la_LIBADD = $(LTLIBOBJS) $(LIBLTDL) $(GLIB_LIBS)

libreindeer_la_LDFLAGS = -export-dynamic -version-info 0:0:0
