# C++ test suite.

AM_DEFAULT_SOURCE_EXT = .cpp

AM_CPPFLAGS = -I../..

check_PROGRAMS =				\
	t-renfxx				\
	t-renfxx-io

LDADD = ../../libnfemb.la
LIBS = -lgmpxx -lgmp

TESTS = $(check_PROGRAMS)
