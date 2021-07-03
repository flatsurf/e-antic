# Installed library (libtool (LT) libraries)
lib_LTLIBRARIES = libeantic.la

libeantic_la_LDFLAGS = -version-info $(libeantic_version_info) -no-undefined

if HAVE_VERSION_SCRIPT
libeantic_la_LDFLAGS += -Wl,--version-script=$(srcdir)/libeantic.map

EXTRA_libeantic_la_DEPENDENCIES = $(srcdir)/libeantic.map
endif

# We are building the libexactreal library so visibility attributes should be
# set, see https://gcc.gnu.org/wiki/Visibility
AM_CPPFLAGS = -DBUILD_LIBEANTIC

# Linked-against libraries
libeantic_la_LIBADD = @LIBS@ $(LTLIBOBJS)

# Installed headers
nobase_pkginclude_HEADERS =      \
		../e-antic/renf.h            \
		../e-antic/renf_elem.h       \
		../e-antic/fmpz_poly_extra.h \
		../e-antic/fmpq_poly_extra.h

libeantic_la_SOURCES =

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


# fmpq_poly_extra/
libeantic_la_SOURCES +=                              \
    fmpq_poly_extra/check_unique_real_root.c         \
    fmpq_poly_extra/evaluate.c                       \
    fmpq_poly_extra/set_str_pretty.c

# renf
libeantic_la_SOURCES +=               \
    renf/clear.c                      \
    renf/degree.c                     \
    renf/equal.c                      \
    renf/get_str.c                    \
    renf/init.c                       \
    renf/init_nth_root.c              \
    renf/init_set.c                   \
    renf/print.c                      \
    renf/randtest.c                   \
    renf/refine_embedding.c           \
    renf/set_embeddings_fmpz_poly.c   \
    renf/set_immutable.c

noinst_HEADERS =                  \
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

# We generate e-antic.h in the Makefile (not in configure) as recommended by
# autoconf so DESTDIR installs work.
$(builddir)/../e-antic/e-antic.h: $(srcdir)/../e-antic/e-antic.h.in Makefile
	mkdir -p $(builddir)/../e-antic
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

# Since we generate e-antic.h, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Generate e-antic.h before performing any compilation
BUILT_SOURCES = ../e-antic/e-antic.h
# Distribute e-antic.h.in as part of the tarball, but not e-antic.h
EXTRA_DIST = ../e-antic/e-antic.h.in
nodist_pkginclude_HEADERS = ../e-antic/e-antic.h

# Install e-antic.h into e-antic/
generatedincludedir = $(includedir)/e-antic
nodist_generatedinclude_HEADERS = ../e-antic/e-antic.h

CLEANFILES = ../e-antic/e-antic.h
