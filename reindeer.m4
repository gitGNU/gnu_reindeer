# Configure paths for Reindeer
#
# Copyright (C) 2008, 2009 - Patrik Olsson
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.

dnl AM_PATH_REINDEER([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for Reindeer, and define REN_CFLAGS and REN_LIBS
dnl
AC_DEFUN([AM_PATH_REINDEER],
[dnl
    no_ren=""

    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

    if test "x$PKG_CONFIG" != xno ; then
    if pkg-config --atleast-pkgconfig-version 0.7 ; then
        :
    else
        echo "*** pkg-config too old; version 0.7 or better required."
        no_ren=yes
        PKG_CONFIG=no
    fi
    else
    no_ren=yes
    fi

    min_ren_version=ifelse([$1], ,0.0.0,$1)
    AC_MSG_CHECKING(for Reindeer - version >= $min_ren_version)

    if test "x$PKG_CONFIG" != xno ; then
        if $PKG_CONFIG --atleast-version $min_ren_version reindeer; then
    	    :
        else
    	    no_ren=yes
        fi
    fi

    if test "x$no_ren" = x ; then
        REN_CFLAGS=`$PKG_CONFIG reindeer --cflags`
        REN_LIBS=`$PKG_CONFIG reindeer --libs`
        ren_config_major_version=`$PKG_CONFIG --modversion reindeer | \
                sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
        ren_config_minor_version=`$PKG_CONFIG --modversion reindeer | \
                sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
        ren_config_micro_version=`$PKG_CONFIG --modversion reindeer | \
                sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    fi
    if test "x$no_ren" = x ; then
        AC_MSG_RESULT(yes (version $ren_config_major_version.$ren_config_minor_version.$ren_config_micro_version))
        ifelse([$2], , :, [$2])
    else
        AC_MSG_RESULT(no)
        if test "$PKG_CONFIG" = "no" ; then
            echo "*** A new enough version of pkg-config was not found."
            echo "*** See http://pkgconfig.sourceforge.net"
        fi
        REN_CFLAGS=""
        REN_LIBS=""
        ifelse([$3], , :, [$3])
    fi
    AC_SUBST(REN_CFLAGS)
    AC_SUBST(REN_LIBS)
])
