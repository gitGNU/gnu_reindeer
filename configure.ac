AC_PREREQ(2.61)
AC_INIT([Reindeer], [0.0.2], , [reindeer])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
LT_CONFIG_LTDL_DIR([libltdl])
AM_INIT_AUTOMAKE
LT_INIT([dlopen])
LTDL_INIT([installable])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.


# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
#AC_C_INLINE

# Checks for library functions.


AC_CONFIG_FILES([
    Makefile
    reindeer.pc
    ren/Makefile
    reindeer/Makefile
])
AC_OUTPUT
