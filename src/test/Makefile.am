# TESTS = whatever
CLEANFILES = whatever
check-local : whatever
whatever_sources = \
    glue.c \
    glue.h \
    testing.go

whatever: libpath = $(realpath @builddir@/../.libs)
whatever: incpath = $(realpath @top_srcdir@/src)
whatever: real_builddir = $(realpath @builddir@)
whatever: ${whatever_sources} @top_builddir@/src/libgsswrap.la
	cd @top_srcdir@/src/test && \
	env CGO_LDFLAGS="-L${libpath} -Wl,-rpath=${libpath}" \
	CGO_CPPFLAGS="-I${incpath}" \
	go build -o ${real_builddir}/$@

EXTRA_DIST = $(whatever_sources)
