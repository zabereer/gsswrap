AC_PREREQ([2.69])
AC_INIT([gsswrap], [1], [nobugs@bunny.org])
# no-portability to use make $(realpath) in Makefile.am for go
AM_INIT_AUTOMAKE([-Wall -Werror -Wno-portability foreign subdir-objects])
AC_CONFIG_SRCDIR([src/gsswrap/common.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# see AC_PROG_CC documentation for removing default CFLAGS
: ${CFLAGS=""}

AC_PROG_CC
AM_PROG_AR

LT_INIT

PKG_WITH_MODULES([KRB5GSSAPI],
                 [krb5-gssapi],
                 ,
                 [AC_MSG_ERROR([sorry but krb5-gssapi is required])])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/test/Makefile])
AC_OUTPUT
