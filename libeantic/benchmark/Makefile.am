noinst_PROGRAMS = benchmark

benchmark_SOURCES = main.cpp renfxx/b-constructor.cpp renfxx/b-arithmetic.cpp

benchmark_LDADD = $(builddir)/../srcxx/libeanticxx.la $(builddir)/../src/libeantic.la

# Include Google's libbenchmark and dependent libraries
benchmark_LDFLAGS = -lbenchmark -lpthread
