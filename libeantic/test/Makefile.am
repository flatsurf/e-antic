if HAVE_BYEXAMPLE
    MAYBE_BYEXAMPLE = byexample
endif

SUBDIRS = $(MAYBE_BYEXAMPLE)

LDADD = ../src/libeantic.la ../srcxx/libeanticxx.la -lgmpxx

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -isystem "@abs_srcdir@/external/cereal/include"

check_PROGRAMS =                                     \
    fmpq_poly_extra/t-evaluate                       \
    fmpq_poly_extra/t-set_str_pretty                 \
    fmpz_poly_extra/t-check_unique_real_root         \
    fmpz_poly_extra/t-has_real_root                  \
    fmpz_poly_extra/t-isolate_real_roots             \
    fmpz_poly_extra/t-num_real_roots                 \
    fmpz_poly_extra/t-num_real_roots_0_1             \
    fmpz_poly_extra/t-num_real_roots_upper_bound     \
    fmpz_poly_extra/t-num_real_roots_vca             \
    fmpz_poly_extra/t-positive_root_upper_bound_2exp \
    fmpz_poly_extra/t-randtest_irreducible           \
    fmpz_poly_extra/t-scale_0_1_fmpq                 \
    fmpz_poly_extra/t-set_str_pretty                 \
    fmpz_poly_extra/t-squarefree_part                \
    renf/t-init                                      \
    renf/t-init_nth_root_fmpq                        \
    renf/t-randtest                                  \
    renf/t-real_embeddings                           \
    renf_elem/t-binop                                \
    renf_elem/t-ceil                                 \
    renf_elem/t-cmp                                  \
    renf_elem/t-cmp_fmpq                             \
    renf_elem/t-equal_fmpq                           \
    renf_elem/t-floor                                \
    renf_elem/t-gen                                  \
    renf_elem/t-get_arb                              \
    renf_elem/t-get_cfrac                            \
    renf_elem/t-get_d                                \
    renf_elem/t-get_str_pretty                       \
    renf_elem/t-is_rational                          \
    renf_elem/t-pow                                  \
    renf_elem/t-swap                                 \
    renfxx/t-assignment                              \
    renfxx/t-binop                                   \
    renfxx/t-ceil                                    \
    renfxx/t-cereal                                  \
    renfxx/t-cmp                                     \
    renfxx/t-constructor                             \
    renfxx/t-floor                                   \
    renfxx/t-get                                     \
    renfxx/t-get_num_den                             \
    renfxx/t-get_str                                 \
    renfxx/t-hash                                    \
    renfxx/t-predicates                              \
    renfxx/t-num_content                             \
    renfxx/t-pow                                     \
    renfxx/t-stream

noinst_HEADERS =                                    \
    rand_generator.hpp                              \
    renf_generator.hpp                              \
    fmpq_generator.hpp                              \
    fmpz_generator.hpp                              \
    renf_elem_generator.hpp                         \
    external/catch2/single_include/catch2/catch.hpp \
    renf_class_generator.hpp                        \
    renf_elem_class_generator.hpp

fmpq_poly_extra_t_evaluate_SOURCES = fmpq_poly_extra/t-evaluate.c
fmpq_poly_extra_t_set_str_pretty_SOURCES = fmpq_poly_extra/t-set_str_pretty.c
fmpz_poly_extra_t_check_unique_real_root_SOURCES = fmpz_poly_extra/t-check_unique_real_root.c
fmpz_poly_extra_t_has_real_root_SOURCES = fmpz_poly_extra/t-has_real_root.c
fmpz_poly_extra_t_isolate_real_roots_SOURCES = fmpz_poly_extra/t-isolate_real_roots.c
fmpz_poly_extra_t_num_real_roots_SOURCES = fmpz_poly_extra/t-num_real_roots.c
fmpz_poly_extra_t_num_real_roots_0_1_SOURCES = fmpz_poly_extra/t-num_real_roots_0_1.c
fmpz_poly_extra_t_num_real_roots_upper_bound_SOURCES = fmpz_poly_extra/t-num_real_roots_upper_bound.c
fmpz_poly_extra_t_num_real_roots_vca_SOURCES = fmpz_poly_extra/t-num_real_roots_vca.c
fmpz_poly_extra_t_positive_root_upper_bound_2exp_SOURCES = fmpz_poly_extra/t-positive_root_upper_bound_2exp.c
fmpz_poly_extra_t_randtest_irreducible_SOURCES = fmpz_poly_extra/t-randtest_irreducible.c
fmpz_poly_extra_t_scale_0_1_fmpq_SOURCES = fmpz_poly_extra/t-scale_0_1_fmpq.c
fmpz_poly_extra_t_set_str_pretty_SOURCES = fmpz_poly_extra/t-set_str_pretty.c
fmpz_poly_extra_t_squarefree_part_SOURCES = fmpz_poly_extra/t-squarefree_part.c
renf_t_init_SOURCES = renf/t-init.c
renf_t_init_nth_root_fmpq_SOURCES = renf/t-init_nth_root_fmpq.c
renf_t_randtest_SOURCES = renf/t-randtest.c
renf_t_real_embeddings_SOURCES = renf/t-real_embeddings.c
renf_elem_t_binop_SOURCES = renf_elem/t-binop.cpp main.cpp
renf_elem_t_ceil_SOURCES = renf_elem/t-ceil.c
renf_elem_t_cmp_SOURCES = renf_elem/t-cmp.c
renf_elem_t_cmp_fmpq_SOURCES = renf_elem/t-cmp_fmpq.c
renf_elem_t_equal_fmpq_SOURCES = renf_elem/t-equal_fmpq.c
renf_elem_t_floor_SOURCES = renf_elem/t-floor.c
renf_elem_t_gen_SOURCES = renf_elem/t-gen.c
renf_elem_t_get_arb_SOURCES = renf_elem/t-get_arb.c
renf_elem_t_get_cfrac_SOURCES = renf_elem/t-get_cfrac.c
renf_elem_t_get_d_SOURCES = renf_elem/t-get_d.cpp main.cpp
renf_elem_t_get_str_pretty_SOURCES = renf_elem/t-get_str_pretty.c
renf_elem_t_is_rational_SOURCES = renf_elem/t-is_rational.c
renf_elem_t_pow_SOURCES = renf_elem/t-pow.c
renf_elem_t_swap_SOURCES = renf_elem/t-swap.c
renfxx_t_assignment_SOURCES = renfxx/t-assignment.cpp main.cpp
renfxx_t_binop_SOURCES = renfxx/t-binop.cpp main.cpp
renfxx_t_ceil_SOURCES = renfxx/t-ceil.cpp
renfxx_t_cmp_SOURCES = renfxx/t-cmp.cpp main.cpp
renfxx_t_cereal_SOURCES = renfxx/t-cereal.cpp main.cpp
renfxx_t_constructor_SOURCES = renfxx/t-constructor.cpp main.cpp
renfxx_t_floor_SOURCES = renfxx/t-floor.cpp
renfxx_t_get_SOURCES = renfxx/t-get.cpp
renfxx_t_get_num_den_SOURCES = renfxx/t-get_num_den.cpp main.cpp
renfxx_t_get_str_SOURCES = renfxx/t-get_str.cpp
renfxx_t_hash_SOURCES = renfxx/t-hash.cpp main.cpp
renfxx_t_predicates_SOURCES = renfxx/t-predicates.cpp main.cpp
renfxx_t_num_content_SOURCES = renfxx/t-num_content.cpp
renfxx_t_pow_SOURCES = renfxx/t-pow.cpp
renfxx_t_stream_SOURCES = renfxx/t-stream.cpp main.cpp

# We vendor the header-only library Cereal (serialization with C++ to be able
# to run the tests even when cereal is not installed.
# Cereal is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST = external/cereal/include

TESTS = $(check_PROGRAMS)
