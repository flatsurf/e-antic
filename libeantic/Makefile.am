if HAVE_BENCHMARK
    MAYBE_BENCHMARK = benchmark
endif

SUBDIRS = src srcxx test $(MAYBE_BENCHMARK)

ACLOCAL_AMFLAGS = -I m4

# Distribute files that we might need to fill/replace from our dependencies
EXTRA_DIST =                                          \
  upstream/patched/EANTIC_fmpq_poly_add_fmpq.c        \
  upstream/patched/EANTIC_nf_elem_add_fmpq.c          \
  upstream/patched/EANTIC_nf_elem_scalar_div_fmpq.c   \
  upstream/patched/fmpq_poly_add_fmpq.c               \
  upstream/patched/nf.h                               \
  upstream/patched/nf_elem.h                          \
  upstream/patched/nf_elem_add_fmpq.c                 \
  upstream/patched/nf_elem_scalar_div_fmpq.c          \
  upstream/patched/nf_init_randtest.c

# Probably a bug in autotools:
# .libs in AC_CONFIG_LIBOBJ_DIR is not cleaned automatically.
mostlyclean-local:
	-rm -rf upstream/patched/.libs
