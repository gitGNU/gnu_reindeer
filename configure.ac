AC_PREREQ(2.61)
AC_INIT([Reindeer], [0.0.2], , [reindeer])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_LIBOBJ_DIR([libgnu])
LT_CONFIG_LTDL_DIR([libltdl])
AM_INIT_AUTOMAKE([subdir-objects])
LT_INIT([dlopen])
LTDL_INIT([nonrecursive installable])

# Checks for programs.
AC_PROG_CC
GNULIB_EARLY

# Checks for libraries.
GLIB_REQUIRED=2.16
AC_SUBST(GLIB_REQUIRED)
PKG_CHECK_MODULES(GLIB, glib-2.0 >= $GLIB_REQUIRED)

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
    reindeer/reindeer-0.0.pc
])
AC_OUTPUT
