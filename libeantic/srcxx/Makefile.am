# Installed library (libtool (LT) libraries)
lib_LTLIBRARIES = libeanticxx.la
libeanticxx_la_LDFLAGS = -version-info @libeanticxx_version_info@ -no-undefined

if HAVE_VERSION_SCRIPT
libeanticxx_la_LDFLAGS += -Wl,--version-script=$(srcdir)/libeanticxx.map

EXTRA_libeanticxx_la_DEPENDENCIES = $(srcdir)/libeanticxx.map
endif

EXTRA_DIST = $(srcdir)/libeanticxx.map

# We are building the libexactreal library so visibility attributes should be
# set, see https://gcc.gnu.org/wiki/Visibility
AM_CPPFLAGS = -DBUILD_LIBEANTIC

# Linked-against libraries
libeanticxx_la_LIBADD = ../src/libeantic.la -lgmpxx -lflint

if HAVE_LIBANTIC
libeanticxx_la_LIBADD += -lantic
endif

# Installed headers
nobase_pkginclude_HEADERS =        \
    ../e-antic/cppyy.hpp           \
    ../e-antic/e-antic.hpp         \
    ../e-antic/forward.hpp         \
    ../e-antic/renfxx.h            \
    ../e-antic/renf_class.hpp      \
    ../e-antic/renf_elem_class.hpp \
    ../e-antic/renfxx_fwd.hpp      \
    ../e-antic/cereal.hpp

libeanticxx_la_SOURCES = \
    renf_class.cpp       \
    renf_elem_class.cpp
