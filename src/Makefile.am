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

libgsswrap_la_CFLAGS = -O3 -g -Wall -Wextra -Werror -pedantic
libgsswrap_la_LIBADD = -lgssapi_krb5

TESTS = whatever
CLEANFILES = whatever
check-local : whatever
whatever_sources = \
    test/testing.go

whatever: libpath = $(realpath @builddir@/.libs)
whatever: ${whatever_sources}
	env CGO_LDFLAGS="-L${libpath} -Wl,-rpath=${libpath}" go build -o $@ $^

EXTRA_DIST = $(whatever_sources)
