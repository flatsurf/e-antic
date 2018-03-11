SUBDIRS = . e-antic/test
AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4

# Installed library
lib_LTLIBRARIES = libeantic.la

# Linked-against libraries
LIBS += -lantic -larb -lflint -lgmp

# Installed headers
nobase_include_HEADERS = e-antic/renf.h e-antic/renf_elem.h e-antic/poly_extra.h
nobase_include_HEADERS += e-antic/renfxx.h

# Initialize variables, to be filled below
noinst_HEADERS =
libeantic_la_SOURCES =
check_PROGRAMS =
dist_doc_DATA =

# poly_extra
libeantic_la_SOURCES +=					\
	poly_extra/arb_add_fmpq.c			\
	poly_extra/bisection_step_arb.c			\
	poly_extra/check_unique_real_root.c		\
	poly_extra/descartes_bound.c			\
	poly_extra/descartes_bound_0_1.c		\
	poly_extra/evaluate.c				\
	poly_extra/fmpz_poly_randtest_irreducible.c	\
	poly_extra/fmpz_poly_scale_0_1_fmpq.c		\
	poly_extra/fmpz_poly_squarefree_part.c		\
	poly_extra/has_real_root.c			\
	poly_extra/newton_step_arb.c			\
	poly_extra/num_real_roots_0_1.c			\
	poly_extra/num_real_roots_0_1_sturm.c		\
	poly_extra/positive_root_upper_bound_2exp.c	\
	poly_extra/real_root_isolation_vca.c		\
	poly_extra/real_roots_isolation.c		\
	poly_extra/sgn_at_half.c

noinst_HEADERS +=				\
	poly_extra/poly_evaluate_template.h

check_PROGRAMS += 						\
	poly_extra/test/t-descartes_bound			\
	poly_extra/test/t-evaluate				\
	poly_extra/test/t-fmpz_poly_scale_0_1_fmpq		\
	poly_extra/test/t-has_real_root				\
	poly_extra/test/t-num_real_roots			\
	poly_extra/test/t-num_real_roots_0_1			\
	poly_extra/test/t-positive_root_upper_bound_2exp	\
	poly_extra/test/t-real_root_isolation			\
	poly_extra/test/t-squarefree_part

dist_doc_DATA +=				\
	README                                  \
	poly_extra/doc/poly_extra.txt

# renf
libeantic_la_SOURCES +=						\
	renf/fmpz_poly_real_embeddings.c			\
	renf/init.c						\
	renf/init_nth_root.c					\
	renf/print.c						\
	renf/randtest.c						\
	renf/refine_embedding.c

check_PROGRAMS +=				\
	renf/test/t-init			\
	renf/test/t-init_nth_root_fmpq		\
	renf/test/t-randtest			\
	renf/test/t-real_embeddings

# renf_elem
libeantic_la_SOURCES +=						\
	renf_elem/ceil.c					\
	renf_elem/check_embedding.c				\
	renf_elem/clear.c					\
	renf_elem/cmp.c						\
	renf_elem/cmp_fmpq.c					\
	renf_elem/floor.c					\
	renf_elem/gen.c                                         \
	renf_elem/get_cfrac.c					\
	renf_elem/get_str_pretty.c				\
	renf_elem/init.c					\
	renf_elem/is_rational.c					\
	renf_elem/print_pretty.c				\
	renf_elem/randtest.c					\
	renf_elem/set_evaluation.c				\
	renf_elem/set_fmpq_poly.c

check_PROGRAMS +=				\
	renf_elem/test/t-binop			\
	renf_elem/test/t-ceil			\
	renf_elem/test/t-cmp			\
	renf_elem/test/t-cmp_fmpq		\
	renf_elem/test/t-floor			\
	renf_elem/test/t-gen                    \
	renf_elem/test/t-get_cfrac		\
	renf_elem/test/t-is_rational

# Run test suite
TESTS = $(check_PROGRAMS)
LDADD = libeantic.la
