noinst_PROGRAMS = benchmark

benchmark_SOURCES = main.cpp renfxx/b-constructor.cpp renfxx/b-arithmetic.cpp renfxx/b-relop.cpp renfxx/b-assignment.cpp

benchmark_LDADD = $(builddir)/../srcxx/libeanticxx.la $(builddir)/../src/libeantic.la

# Include Google's libbenchmark and dependent libraries
benchmark_LDFLAGS = -lbenchmark -lpthread

noinst_HEADERS =                               \
		renfxx/number_fields.hpp                   \
		renfxx/renf_elem_class_pool.hpp
