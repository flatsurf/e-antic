AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4
# Installed library (libtool (LT) libraries)
lib_LTLIBRARIES = libeantic.la libeanticxx.la
libeantic_la_LDFLAGS = -version-info @libeantic_version_info@ -no-undefined
libeanticxx_la_LDFLAGS = -version-info @libeanticxx_version_info@ -no-undefined

# Linked-against libraries
libeantic_la_LIBADD = @LIBS@ $(LTLIBOBJS)
libeanticxx_la_LIBADD = libeantic.la -lgmpxx -lflint -lantic

# Installed headers
nobase_include_HEADERS = e-antic/renf.h e-antic/renf_elem.h e-antic/fmpz_poly_extra.h e-antic/fmpq_poly_extra.h e-antic/renfxx.h e-antic/renfxx_fwd.h e-antic/renfxx_cereal.h

# Put config.h on the search path, https://www.gnu.org/software/automake/manual/html_node/VPATH-Builds.html
AM_CPPFLAGS = -I.

# Initialize variables, to be filled below
noinst_HEADERS =
noinst_PROGRAMS =
libeantic_la_SOURCES =
check_PROGRAMS =

EXTRA_DIST =

LDADD = libeantic.la

@VALGRIND_CHECK_RULES@

# fmpz_poly_extra/
libeantic_la_SOURCES +=                              \
    fmpz_poly_extra/bisection_step_arb.c             \
    fmpz_poly_extra/descartes_bound.c                \
    fmpz_poly_extra/descartes_bound_0_1.c            \
    fmpz_poly_extra/evaluate.c                       \
    fmpz_poly_extra/randtest_irreducible.c           \
    fmpz_poly_extra/scale_0_1_fmpq.c                 \
    fmpz_poly_extra/squarefree_part.c                \
    fmpz_poly_extra/has_real_root.c                  \
    fmpz_poly_extra/newton_step_arb.c                \
    fmpz_poly_extra/num_real_roots_0_1.c             \
    fmpz_poly_extra/num_real_roots_0_1_sturm.c       \
    fmpz_poly_extra/positive_root_upper_bound_2exp.c \
    fmpz_poly_extra/isolate_real_roots.c             \
    fmpz_poly_extra/relative_condition_number_2exp.c \
    fmpz_poly_extra/abs.c                            \
    fmpz_poly_extra/evaluate_at_one.c                \
    fmpz_poly_extra/num_real_roots_0_1_vca.c         \
    fmpz_poly_extra/num_real_roots_vca.c             \
    fmpz_poly_extra/set_str_pretty.c

check_PROGRAMS +=                                         \
    fmpz_poly_extra/test/t-check_unique_real_root         \
    fmpz_poly_extra/test/t-has_real_root                  \
    fmpz_poly_extra/test/t-isolate_real_roots             \
    fmpz_poly_extra/test/t-num_real_roots                 \
    fmpz_poly_extra/test/t-num_real_roots_0_1             \
    fmpz_poly_extra/test/t-num_real_roots_upper_bound     \
    fmpz_poly_extra/test/t-num_real_roots_vca             \
    fmpz_poly_extra/test/t-positive_root_upper_bound_2exp \
    fmpz_poly_extra/test/t-randtest_irreducible           \
    fmpz_poly_extra/test/t-scale_0_1_fmpq                 \
    fmpz_poly_extra/test/t-set_str_pretty                 \
    fmpz_poly_extra/test/t-squarefree_part


# fmpq_poly_extra/
libeantic_la_SOURCES +=                              \
    fmpq_poly_extra/check_unique_real_root.c         \
    fmpq_poly_extra/evaluate.c                       \
    fmpq_poly_extra/set_str_pretty.c

check_PROGRAMS +=                                         \
    fmpq_poly_extra/test/t-evaluate                       \
    fmpq_poly_extra/test/t-set_str_pretty

# renf
libeantic_la_SOURCES +=               \
    renf/clear.c                      \
    renf/degree.c                     \
    renf/equal.c                      \
    renf/fmpz_poly_real_embeddings.c  \
    renf/get_str.c                    \
    renf/init.c                       \
    renf/init_nth_root.c              \
    renf/init_set.c                   \
    renf/print.c                      \
    renf/randtest.c                   \
    renf/refine_embedding.c           \
    renf/set_immutable.c

check_PROGRAMS +=                  \
    renf/test/t-init               \
    renf/test/t-init_nth_root_fmpq \
    renf/test/t-randtest           \
    renf/test/t-real_embeddings

noinst_HEADERS +=                  \
    renf_elem/rounding_template.h

# renf_elem
libeantic_la_SOURCES +=                        \
    renf_elem/add.c                            \
    renf_elem/add_fmpq.c                       \
    renf_elem/add_fmpz.c                       \
    renf_elem/add_si.c                         \
    renf_elem/add_ui.c                         \
    renf_elem/check_embedding.c                \
    renf_elem/clear.c                          \
    renf_elem/cmp.c                            \
    renf_elem/cmp_fmpq.c                       \
    renf_elem/cmp_fmpz.c                       \
    renf_elem/cmp_si.c                         \
    renf_elem/cmp_ui.c                         \
    renf_elem/div.c                            \
    renf_elem/div_fmpq.c                       \
    renf_elem/div_fmpz.c                       \
    renf_elem/div_si.c                         \
    renf_elem/div_ui.c                         \
    renf_elem/equal.c                          \
    renf_elem/equal_fmpq.c                     \
    renf_elem/equal_fmpz.c                     \
    renf_elem/equal_si.c                       \
    renf_elem/equal_ui.c                       \
    renf_elem/fdiv.c                           \
    renf_elem/floor_ceil.c                     \
    renf_elem/fmpq_div.c                       \
    renf_elem/fmpq_sub.c                       \
    renf_elem/gen.c                            \
    renf_elem/get_arb.c                        \
    renf_elem/get_cfrac.c                      \
    renf_elem/get_d.c                          \
    renf_elem/get_str_pretty.c                 \
    renf_elem/init.c                           \
    renf_elem/inv.c                            \
    renf_elem/is_integer.c                     \
    renf_elem/is_one.c                         \
    renf_elem/is_rational.c                    \
    renf_elem/is_zero.c                        \
    renf_elem/mul.c                            \
    renf_elem/mul_fmpq.c                       \
    renf_elem/mul_fmpz.c                       \
    renf_elem/mul_si.c                         \
    renf_elem/mul_ui.c                         \
    renf_elem/neg.c                            \
    renf_elem/one.c                            \
    renf_elem/pow.c                            \
    renf_elem/print_pretty.c                   \
    renf_elem/randtest.c                       \
    renf_elem/relative_condition_number_2exp.c \
    renf_elem/set.c                            \
    renf_elem/set_evaluation.c                 \
    renf_elem/set_fmpq.c                       \
    renf_elem/set_fmpq_poly.c                  \
    renf_elem/set_fmpz.c                       \
    renf_elem/set_mpq.c                        \
    renf_elem/set_mpz.c                        \
    renf_elem/set_nf_elem.c                    \
    renf_elem/set_si.c                         \
    renf_elem/set_ui.c                         \
    renf_elem/sgn.c                            \
    renf_elem/sub.c                            \
    renf_elem/sub_fmpq.c                       \
    renf_elem/sub_fmpz.c                       \
    renf_elem/sub_si.c                         \
    renf_elem/sub_ui.c                         \
    renf_elem/zero.c

check_PROGRAMS +=                    \
    renf_elem/test/t-ceil            \
    renf_elem/test/t-cmp             \
    renf_elem/test/t-cmp_fmpq        \
    renf_elem/test/t-equal_fmpq      \
    renf_elem/test/t-floor           \
    renf_elem/test/t-gen             \
    renf_elem/test/t-get_arb         \
    renf_elem/test/t-get_cfrac       \
    renf_elem/test/t-get_str_pretty  \
    renf_elem/test/t-is_rational     \
    renf_elem/test/t-pow             \
    renf_elem/test/t-swap

# C tests written with catch2, a C++ testing framework

check_PROGRAMS +=                \
    renf_elem/test/t-binop       \
    renf_elem/test/t-get_d

renf_elem_test_t_get_d_SOURCES = renfxx/test/main.cpp renf_elem/test/t-get_d.cpp
renf_elem_test_t_get_d_LDADD = libeantic.la
renf_elem_test_t_binop_SOURCES = renfxx/test/main.cpp renf_elem/test/t-binop.cpp
renf_elem_test_t_binop_LDADD = libeantic.la

noinst_HEADERS +=                                               \
    renf/test/rand_generator.hpp                                \
    renf/test/renf_generator.hpp                                \
    renf_elem/test/fmpq_generator.hpp                           \
    renf_elem/test/fmpz_generator.hpp                           \
    renf_elem/test/renf_elem_generator.hpp                      \
    renfxx/test/external/catch2/single_include/catch2/catch.hpp

# C++

libeanticxx_la_SOURCES =       \
    renfxx/renf_class.cpp      \
    renfxx/renf_elem_class.cpp

noinst_HEADERS +=                                     \
    renfxx/external/unique-factory/unique_factory.hpp \
    renfxx/external/unique-factory/unique_factory.hpp \
    renfxx/test/renf_class_generator.hpp              \
    renfxx/test/renf_elem_class_generator.hpp

# We generate e-antic.h in the Makefile (not in configure) as recommended by
# autoconf to DESTDIR installs work.
$(builddir)/e-antic/e-antic.h: $(srcdir)/e-antic/e-antic.h.in Makefile
	mkdir -p $(builddir)/e-antic
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

# Since we generate e-antic.h, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Generate e-antic.h before performing any compilation
BUILT_SOURCES = e-antic/e-antic.h
# Distribute e-antic.h.in as part of the tarball, but not e-antic.h
EXTRA_DIST += e-antic/e-antic.h.in
nodist_pkginclude_HEADERS = e-antic/e-antic.h

# Distribute files that we might need to fill/replace from our dependencies
EXTRA_DIST +=                                                     \
						  upstream/patched/EANTIC_fmpq_poly_add_fmpq.c        \
              upstream/patched/EANTIC_nf_elem_add_fmpq.c          \
              upstream/patched/EANTIC_nf_elem_scalar_div_fmpq.c   \
              upstream/patched/fmpq_poly_add_fmpq.c               \
              upstream/patched/nf.h                               \
              upstream/patched/nf_elem.h                          \
              upstream/patched/nf_elem_add_fmpq.c                 \
              upstream/patched/nf_elem_scalar_div_fmpq.c          \
              upstream/patched/nf_init_randtest.c

CLEANFILES = e-antic/e-antic.h

# Probably a bug in autotools:
# .libs in AC_CONFIG_LIBOBJ_DIR is not cleaned automatically.
mostlyclean-local:
	-rm -rf upstream/patched/.libs

# We vendor the header-only library Cereal (serialization of the C++
# structures) to be able to run the tests even when cereal is not installed.
# Cereal is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their sourc tree.
EXTRA_DIST += renfxx/test/external/cereal/include

check_PROGRAMS +=                     \
    renfxx/test/t-assignment          \
    renfxx/test/t-binop               \
    renfxx/test/t-ceil                \
    renfxx/test/t-cereal              \
    renfxx/test/t-cmp                 \
    renfxx/test/t-constructor         \
    renfxx/test/t-floor               \
    renfxx/test/t-get                 \
    renfxx/test/t-get_num_den         \
    renfxx/test/t-get_str             \
    renfxx/test/t-hash                \
    renfxx/test/t-is_integer_rational \
    renfxx/test/t-num_content         \
    renfxx/test/t-pow                 \
    renfxx/test/t-stream

# NOTE: ideally we should not provide explicit source for the
# check programs. However, automake seems confused with the
# mix of C and C++ files.
renfxx_test_t_assignment_LDADD = libeanticxx.la libeantic.la
renfxx_test_t_assignment_SOURCES = renfxx/test/main.cpp renfxx/test/t-assignment.cpp
renfxx_test_t_binop_LDADD = libeanticxx.la libeantic.la -lgmpxx
renfxx_test_t_binop_SOURCES = renfxx/test/main.cpp renfxx/test/t-binop.cpp
renfxx_test_t_ceil_LDADD = libeanticxx.la libeantic.la -lgmpxx
renfxx_test_t_ceil_SOURCES = renfxx/test/t-ceil.cpp
renfxx_test_t_cereal_CPPFLAGS = -isystem $(srcdir)/renfxx/test/external/cereal/include
renfxx_test_t_cereal_LDADD = libeanticxx.la libeantic.la
renfxx_test_t_cereal_SOURCES = renfxx/test/main.cpp renfxx/test/t-cereal.cpp
renfxx_test_t_cmp_LDADD = libeanticxx.la libeantic.la
renfxx_test_t_cmp_SOURCES = renfxx/test/t-cmp.cpp
renfxx_test_t_constructor_LDADD = libeanticxx.la libeantic.la
renfxx_test_t_constructor_SOURCES = renfxx/test/main.cpp renfxx/test/t-constructor.cpp
renfxx_test_t_floor_LDADD = libeanticxx.la libeantic.la -lgmpxx
renfxx_test_t_floor_SOURCES = renfxx/test/t-floor.cpp
renfxx_test_t_get_LDADD = libeanticxx.la libeantic.la
renfxx_test_t_get_SOURCES = renfxx/test/t-get.cpp
renfxx_test_t_get_num_den_LDADD = libeanticxx.la libeantic.la -lgmpxx
renfxx_test_t_get_num_den_SOURCES = renfxx/test/main.cpp renfxx/test/t-get_num_den.cpp
renfxx_test_t_get_str_LDADD = libeanticxx.la libeantic.la
renfxx_test_t_get_str_SOURCES = renfxx/test/t-get_str.cpp
renfxx_test_t_hash_LDADD = libeanticxx.la libeantic.la -lgmpxx
renfxx_test_t_hash_SOURCES = renfxx/test/main.cpp renfxx/test/t-hash.cpp
renfxx_test_t_is_integer_rational_LDADD = libeanticxx.la libeantic.la -lgmpxx
renfxx_test_t_is_integer_rational_SOURCES = renfxx/test/main.cpp renfxx/test/t-is_integer_rational.cpp
renfxx_test_t_num_content_LDADD = libeanticxx.la libeantic.la -lgmpxx
renfxx_test_t_num_content_SOURCES = renfxx/test/t-num_content.cpp
renfxx_test_t_pow_LDADD = libeanticxx.la libeantic.la -lgmpxx
renfxx_test_t_pow_SOURCES = renfxx/test/t-pow.cpp
renfxx_test_t_stream_LDADD = libeanticxx.la libeantic.la
renfxx_test_t_stream_SOURCES = renfxx/test/main.cpp renfxx/test/t-stream.cpp

TESTS = $(check_PROGRAMS)

if HAVE_BENCHMARK

noinst_PROGRAMS += renfxx/benchmark/benchmark

renfxx_benchmark_benchmark_SOURCES = renfxx/benchmark/main.cpp renfxx/benchmark/b-constructor.cpp

renfxx_benchmark_benchmark_LDADD = $(builddir)/libeanticxx.la $(builddir)/libeantic.la
# Include Google's libbenchmark and dependent libraries
renfxx_benchmark_benchmark_LDFLAGS = -lbenchmark -lrt -lpthread

endif
