# C++ test suite.

AM_DEFAULT_SOURCE_EXT = .cpp

AM_CPPFLAGS = -I$(top_srcdir)

check_PROGRAMS =				\
	t-renfxx				\
	t-renfxx_constructor                    \
	t-renfxx_io                             \
	t-renfxx_assignment                     \
	t-renfxx_cmp                            \
	t-renfxx_binop                          \
	t-renfxx_is_integer_rational            \
	t-renfxx_floor                          \
	t-renfxx_ceil

LDADD = $(top_builddir)/libeantic.la
LIBS = -lgmpxx -lgmp

TESTS = $(check_PROGRAMS)
