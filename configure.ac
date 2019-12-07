AC_PREREQ([2.69])
AC_INIT([gsswrap], [1], [nobugs@bunny.org])
# AM_INIT_AUTOMAKE([-Wall -Werror -Wno-portability foreign subdir-objects])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_SRCDIR([src/gsswrap/common.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# see AC_PROG_CC documentation for removing default CFLAGS
: ${CFLAGS=""}

AC_PROG_CC
# AC_PROG_GO([go gccgo])
# # AC_PROG_GO documentation lies about setting GOFLAGS before AC_PROG_GO
# GOFLAGS=""
AM_PROG_AR

LT_INIT

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
