lib_LTLIBRARIES = libgsswrap.la

include_HEADERS = \
    gsswrap/callbacks.h \
    gsswrap/client.h \
    gsswrap/common.h \
    gsswrap/server.h

libgsswrap_la_SOURCES = \
    gsswrap/client.c \
    gsswrap/common.c \
    gsswrap/context.c \
    gsswrap/context.h \
    gsswrap/credential.c \
    gsswrap/credential.h \
    gsswrap/server.c \
    gsswrap/status.c \
    gsswrap/status.h

libgsswrap_la_CFLAGS = \
    @KRB5GSSAPI_CFLAGS@ \
    -O3 -g -Wall -Wextra -Werror -pedantic
libgsswrap_la_LIBADD = \
    @KRB5GSSAPI_LIBS@

# TESTS = whatever
CLEANFILES = whatever
check-local : whatever
whatever_sources = \
    test/glue.c \
    test/glue.h \
    test/testing.go

whatever: libpath = $(realpath @builddir@/.libs)
whatever: incpath = $(realpath @top_srcdir@/src)
whatever: real_builddir = $(realpath @builddir@)
whatever: ${whatever_sources} libgsswrap.la
	cd @top_srcdir@/src/test && \
	env CGO_LDFLAGS="-L${libpath} -Wl,-rpath=${libpath}" \
	CGO_CPPFLAGS="-I${incpath}" \
	go build -o ${real_builddir}/$@

EXTRA_DIST = $(whatever_sources)
