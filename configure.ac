AC_PREREQ([2.64])
LT_PREREQ([2.2.6])

m4_define([ren_major_version], [0])
m4_define([ren_minor_version], [2])
m4_define([ren_micro_version], [0])
m4_define([ren_version],
	[ren_major_version.ren_minor_version.ren_micro_version])
m4_define([ren_compat_version], [0.2])

AC_INIT([Reindeer], [ren_version], [http://savannah.nongnu.org/bugs/?group=reindeer], [reindeer], [http://savannah.nongnu.org/projects/reindeer/])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_LIBOBJ_DIR([libgnu])
LT_CONFIG_LTDL_DIR([libltdl])
AM_INIT_AUTOMAKE([1.11.1 subdir-objects])
AM_SILENT_RULES([yes])
LT_INIT([dlopen])
LTDL_INIT([nonrecursive installable])

AC_SUBST([REN_MAJOR_VERSION], [ren_major_version])
AC_SUBST([REN_MINOR_VERSION], [ren_minor_version])
AC_SUBST([REN_MICRO_VERSION], [ren_micro_version])
AC_SUBST([REN_COMPAT_VERSION], [ren_compat_version])

renlibdir='${libdir}/reindeer-0.2'
AC_SUBST([renlibdir])

# Checks for programs.
AC_PROG_CC
GNULIB_EARLY
AX_PROG_XSLTPROC([--nonet])
AC_CHECK_DOCBOOK_XSLT
AM_CONDITIONAL([HAVE_DOCBOOK_XSLT], [test x$XSLTPROC != x -a x$HAVE_DOCBOOK_XSLT = xyes])

# Checks for libraries.
GLIB_REQUIRED=2.16
AC_SUBST([GLIB_REQUIRED])
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= $GLIB_REQUIRED])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE

# Checks for library functions.
GNULIB_INIT

# Output
AC_CONFIG_FILES([
	Makefile
	libgnu/Makefile
	reindeer/reindeer-0.2.pc
	ren/version.h
])
AC_OUTPUT
