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
libgsswrap_la_LDFLAGS = -version-info $(subst .,:,@PACKAGE_VERSION@)
libgsswrap_la_LIBADD = \
    @KRB5GSSAPI_LIBS@

pkgconfig_DATA = gsswrap.pc

CLEANFILES = tester
check-local : tester
tester_sources = \
    test/glue.c \
    test/glue.h \
    test/tester.go

tester: libpath = $(realpath @builddir@/.libs)
tester: incpath = $(realpath @top_srcdir@/src)
tester: real_builddir = $(realpath @builddir@)
tester: ${tester_sources} libgsswrap.la
	cd @top_srcdir@/src/test && \
	env CGO_LDFLAGS="-L${libpath} -Wl,-rpath=${libpath}" \
	CGO_CPPFLAGS="-I${incpath}" \
	go build -o ${real_builddir}/$@

EXTRA_DIST = $(tester_sources)
