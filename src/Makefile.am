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
    test/testing.go

whatever: libpath = $(realpath @builddir@/.libs)
whatever: incpath = $(realpath @srcdir@)
whatever: ${whatever_sources} libgsswrap.la
	env CGO_LDFLAGS="-L${libpath} -Wl,-rpath=${libpath}" \
	CGO_CPPFLAGS="-I${incpath}" \
	go build -o $@ $(filter-out libgsswrap.la,$^)
# note using $^ to get path names to sources in srcdir, if
# ${whatever_sources} was uses then VPATH builds would not work

EXTRA_DIST = $(whatever_sources)
