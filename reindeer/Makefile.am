AM_CPPFLAGS     =   -I$(top_builddir) -I$(top_srcdir) \
                    $(LTDLINCL)

lib_LTLIBRARIES                 =   libreindeer.la
libreindeer_la_SOURCES          =   reindeer.c reindeer.h backend.h \
                                    functable.h impl-macros.h error.c util.c \
                                    datablock.c vertexarray.c vertexarray.h \
                                    coordarray.c colorarray.c normalarray.c \
                                    indexarray.c \
                                    light.c
libreindeer_la_LIBADD           =   @LTLIBOBJS@ $(LIBLTDL)
libreindeer_la_LDFLAGS          =   -version-info 0:0:0
