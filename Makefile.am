AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = $(OPENMP_CFLAGS)

# Installed library (libtool (LT) libraries)
lib_LTLIBRARIES = libeantic.la libeanticxx.la

# Linked-against libraries
libeantic_la_LIBADD = -larb -lflint -lgmp
libeanticxx_la_LIBADD = libeantic.la -larb -lflint -lgmpxx -lgmp

# Installed headers
nobase_include_HEADERS = e-antic/e-antic.h e-antic/nf.h e-antic/nf_elem.h e-antic/renf.h e-antic/renf_elem.h e-antic/poly_extra.h e-antic/renfxx.h

# Initialize variables, to be filled below
noinst_HEADERS =
libeantic_la_SOURCES =
check_PROGRAMS =
dist_doc_DATA =

# TODO: we currently build all tests with linked c++ libraries
LDADD = libeanticxx.la libeantic.la -larb -lflint -lgmpxx -lgmp

@VALGRIND_CHECK_RULES@

# poly_extra
libeantic_la_SOURCES +=                         \
    poly_extra/arb_add_fmpq.c                   \
    poly_extra/bisection_step_arb.c             \
    poly_extra/check_unique_real_root.c         \
    poly_extra/descartes_bound.c                \
    poly_extra/descartes_bound_0_1.c            \
    poly_extra/evaluate.c                       \
    poly_extra/fmpz_poly_randtest_irreducible.c \
    poly_extra/fmpz_poly_scale_0_1_fmpq.c       \
    poly_extra/fmpz_poly_squarefree_part.c      \
    poly_extra/fmpq_poly_set_str_pretty.c       \
    poly_extra/has_real_root.c                  \
    poly_extra/newton_step_arb.c                \
    poly_extra/num_real_roots_0_1.c             \
    poly_extra/num_real_roots_0_1_sturm.c       \
    poly_extra/fmpz_poly_positive_root_upper_bound_2exp.c \
    poly_extra/fmpz_poly_isolate_real_roots.c     \
    poly_extra/sgn_at_half.c
if FLINT252
libeantic_la_SOURCES +=                           \
    poly_extra/fmpz_poly_product_roots_fmpq_vec.c \
    poly_extra/fmpz_poly_randtest_no_real_root.c  \
    poly_extra/fmpz_nextprime.c                   \
    poly_extra/fmpz_randprime.c                   \
    poly_extra/fmpq_poly_resultant_modular_div.c  \
    poly_extra/fmpq_poly_inlines.c                \
    poly_extra/fmpz_poly_num_real_roots.c         \
    poly_extra/fmpz_poly_num_real_roots_sturm.c   \
    poly_extra/fmpz_poly_resultant_modular_div.c  \
    poly_extra/fmpz_poly_scale_2exp.c             \
    poly_extra/fmpq_set_str.c                     \
    poly_extra/fmpq_vec_fprint.c                  \
    poly_extra/fmpq_vec_randtest_uniq_sorted.c    \
    poly_extra/fmpq_vec_sort.c                    \
    poly_extra/fmpz_poly_remove_content_2exp.c    \
    poly_extra/fmpq_poly_get_str_pretty.c
endif

noinst_HEADERS +=                    \
    poly_extra/poly_evaluate_template.h

check_PROGRAMS +=                                    \
    poly_extra/test/t-evaluate                       \
    poly_extra/test/t-fmpz_poly_scale_0_1_fmpq       \
    poly_extra/test/t-descartes_bound                \
    poly_extra/test/t-fmpz_poly_positive_root_upper_bound_2exp \
    poly_extra/test/t-has_real_root                  \
    poly_extra/test/t-num_real_roots                 \
    poly_extra/test/t-num_real_roots_sturm           \
    poly_extra/test/t-num_real_roots_vca             \
    poly_extra/test/t-num_real_roots_0_1             \
    poly_extra/test/t-real_root_isolation            \
    poly_extra/test/t-squarefree_part                \
    poly_extra/test/t-check_unique_real_root         \
    poly_extra/test/t-fmpq_poly_set_str_pretty       \
    poly_extra/test/t-fmpz_poly_randtest_irreducible
if FLINT252
check_PROGRAMS +=                                    \
    poly_extra/test/t-nextprime                      \
    poly_extra/test/t-randprime                      \
    poly_extra/test/t-fmpq_poly_resultant_div        \
    poly_extra/test/t-fmpz_poly_resultant_modular_div \
    poly_extra/test/t-fmpz_poly_scale_2exp           \
    poly_extra/test/t-fmpq_get_set_str               \
    poly_extra/test/t-fmpq_vec_randtest_uniq_sorted
endif


dist_doc_DATA +=                   \
    README                         \
    poly_extra/doc/poly_extra.txt

# nf
libeantic_la_SOURCES +=               \
    nf/init.c                         \
    nf/clear.c                        \
    nf/print.c                        \
    nf/init_randtest.c

check_PROGRAMS +=                     \
    nf/test/t-init_clear

# nf_elem
libeantic_la_SOURCES +=               \
    nf_elem/add.c                     \
    nf_elem/add_fmpq.c                \
    nf_elem/add_fmpz.c                \
    nf_elem/add_si.c                  \
    nf_elem/clear.c                   \
    nf_elem/div.c                     \
    nf_elem/equal.c                   \
    nf_elem/fmpq_sub.c                \
    nf_elem/fmpz_sub.c                \
    nf_elem/get_fmpz_mat_row.c        \
    nf_elem/get_fmpz_mod_poly.c       \
    nf_elem/get_nmod_poly.c           \
    nf_elem/init.c                    \
    nf_elem/inlines.c                 \
    nf_elem/inv.c                     \
    nf_elem/invertible_check.c        \
    nf_elem/mod_fmpz.c                \
    nf_elem/mul.c                     \
    nf_elem/norm.c                    \
    nf_elem/norm_div.c                \
    nf_elem/pow.c                     \
    nf_elem/print_pretty.c            \
    nf_elem/randtest.c                \
    nf_elem/reduce.c                  \
    nf_elem/rep_mat.c                 \
    nf_elem/rep_mat_fmpz_mat_den.c    \
    nf_elem/scalar_div_fmpq.c         \
    nf_elem/scalar_div_fmpz.c         \
    nf_elem/scalar_div_si.c           \
    nf_elem/scalar_mul_fmpq.c         \
    nf_elem/scalar_mul_fmpz.c         \
    nf_elem/scalar_mul_si.c           \
    nf_elem/set_fmpz_mat_row.c        \
    nf_elem/si_sub.c                  \
    nf_elem/sub.c                     \
    nf_elem/sub_fmpq.c                \
    nf_elem/sub_fmpz.c                \
    nf_elem/sub_si.c                  \
    nf_elem/trace.c

check_PROGRAMS +=                          \
    nf_elem/test/t-add_sub                 \
    nf_elem/test/t-div                     \
    nf_elem/test/t-equal_fmpz_fmpq         \
    nf_elem/test/t-get_fmpz_mod_poly       \
    nf_elem/test/t-get_nmod_poly           \
    nf_elem/test/t-get_set_den             \
    nf_elem/test/t-get_set_fmpq_poly       \
    nf_elem/test/t-get_set_fmpz_mat_row    \
    nf_elem/test/t-get_str_pretty          \
    nf_elem/test/t-init_clear              \
    nf_elem/test/t-inv                     \
    nf_elem/test/t-is_rational_integer     \
    nf_elem/test/t-mod_fmpz                \
    nf_elem/test/t-mul                     \
    nf_elem/test/t-mul_gen                 \
    nf_elem/test/t-norm                    \
    nf_elem/test/t-norm_div                \
    nf_elem/test/t-pow                     \
    nf_elem/test/t-rep_mat                 \
    nf_elem/test/t-rep_mat_fmpz_mat_den    \
    nf_elem/test/t-set_coeff_num_fmpz      \
    nf_elem/test/t-set_equal               \
    nf_elem/test/t-set_equal_si_ui         \
    nf_elem/test/t-trace

# renf
libeantic_la_SOURCES +=               \
    renf/fmpz_poly_real_embeddings.c  \
    renf/init.c                       \
    renf/init_set.c                   \
    renf/init_nth_root.c              \
    renf/print.c                      \
    renf/randtest.c                   \
    renf/refine_embedding.c

check_PROGRAMS +=                  \
    renf/test/t-init               \
    renf/test/t-init_nth_root_fmpq \
    renf/test/t-randtest           \
    renf/test/t-real_embeddings

# renf_elem
libeantic_la_SOURCES +=            \
    renf_elem/ceil.c               \
    renf_elem/check_embedding.c    \
    renf_elem/clear.c              \
    renf_elem/cmp.c                \
    renf_elem/cmp_fmpq.c           \
    renf_elem/floor.c              \
    renf_elem/gen.c                \
    renf_elem/get_cfrac.c          \
    renf_elem/get_str_pretty.c     \
    renf_elem/init.c               \
    renf_elem/print_pretty.c       \
    renf_elem/randtest.c           \
    renf_elem/set_evaluation.c     \
    renf_elem/set_fmpq_poly.c      \
    renf_elem/sgn.c                \
    renf_elem/get_d.c

check_PROGRAMS +=                \
    renf_elem/test/t-binop       \
    renf_elem/test/t-ceil        \
    renf_elem/test/t-equal_fmpq  \
    renf_elem/test/t-cmp         \
    renf_elem/test/t-cmp_fmpq    \
    renf_elem/test/t-floor       \
    renf_elem/test/t-gen         \
    renf_elem/test/t-get_cfrac   \
    renf_elem/test/t-is_rational \
    renf_elem/test/t-get_d       \
    renf_elem/test/t-get_str_pretty

# C++

libeanticxx_la_SOURCES =        \
    renfxx/get_num_vector.cpp   \
    renfxx/set_renf_xalloc.cpp

check_PROGRAMS +=                     \
    renfxx/test/t-constructor         \
    renfxx/test/t-get_str             \
    renfxx/test/t-get                 \
    renfxx/test/t-assignment          \
    renfxx/test/t-get_num_den         \
    renfxx/test/t-cmp                 \
    renfxx/test/t-binop               \
    renfxx/test/t-is_integer_rational \
    renfxx/test/t-floor               \
    renfxx/test/t-ceil                \
    renfxx/test/t-stream

# NOTE: ideally we should not provide explicit source for the
# check programs. However, automake seems confused with the
# mix of C and C++ files.
renfxx_test_t_constructor_SOURCES = renfxx/test/t-constructor.cpp
renfxx_test_t_get_SOURCES = renfxx/test/t-get.cpp
renfxx_test_t_assignment_SOURCES = renfxx/test/t-assignment.cpp
renfxx_test_t_get_num_den_SOURCES = renfxx/test/t-get_num_den.cpp
renfxx_test_t_cmp_SOURCES = renfxx/test/t-cmp.cpp
renfxx_test_t_binop_SOURCES = renfxx/test/t-binop.cpp
renfxx_test_t_is_integer_rational_SOURCES = renfxx/test/t-is_integer_rational.cpp
renfxx_test_t_floor_SOURCES = renfxx/test/t-floor.cpp
renfxx_test_t_ceil_SOURCES = renfxx/test/t-ceil.cpp
renfxx_test_t_get_str_SOURCES = renfxx/test/t-get_str.cpp
renfxx_test_t_stream_SOURCES = renfxx/test/t-stream.cpp

# test suite
TESTS = $(check_PROGRAMS)
