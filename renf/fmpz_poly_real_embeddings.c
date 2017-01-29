/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "renf.h"
#include "poly_extra.h"

slong renf_set_embeddings_fmpz_poly(renf * nf, fmpz_poly_t pol, slong lim, slong prec)
{
    slong i, n, n_exact, n_interval;
    fmpq_poly_t p2;
    arb_t a;
    fmpz * c;
    slong * k;

    n = fmpz_poly_num_real_roots_upper_bound(pol);
#ifdef DEBUG
    flint_printf("[renf_set_embeddings_fmpz_poly] n = %wd\n", n);
#endif


    c = _fmpz_vec_init(n);
    k = (slong *) flint_malloc(n * sizeof(slong));

#ifdef DEBUG
    printf("[renf_set_embeddings_fmpz_poly] isolating...\n"); fflush(stdout);
#endif
    fmpz_poly_isolate_real_roots(NULL, &n_exact, c, k, &n_interval, pol);
#ifdef DEBUG
    printf("[renf_set_embeddings_fmpz_poly] isolating done\n");
    flint_printf("[renf_set_embeddings_fmpz_poly] n_exact = %wd\n", n_exact);
    flint_printf("[renf_set_embeddings_fmpz_poly] n_interval = %wd\n", n_interval);
    fflush(stdout);
#endif

    if (n_exact)
    {
        fprintf(stderr, "ERROR (fmpz_poly_real_embeddings): rational roots\n");
        flint_abort();
    }

    arb_init(a);
    fmpq_poly_init(p2);
    fmpz_one(fmpq_poly_denref(p2));
    fmpq_poly_fit_length(p2, pol->length);
    _fmpz_vec_set(p2->coeffs, pol->coeffs, pol->length);
    p2->length = pol->length;
    for (i = 0; i < FLINT_MIN(lim, n_interval); i++)
    {
#ifdef DEBUG
        flint_printf("[renf_set_embeddings_fmpz_poly] i = %wd\n", i);
        flint_printf("[renf_set_embeddings_fmpz_poly] c[i] =");
        fmpz_print(c + i);
        printf("\n");
        flint_printf("[renf_set_embeddings_fmpz_poly] k[i] = %wd\n", k[i]);
        fflush(stdout);
#endif

        arb_set_fmpz(a, c + i);
        arb_mul_2exp_si(a, a, 1);
        arb_add_si(a, a, 1, prec);
        mag_one(arb_radref(a));
        arb_mul_2exp_si(a, a, k[i] - 1);
#ifdef DEBUG
        flint_printf("[renf_set_embeddings_fmpz_poly] calling renf_init\n", i);
        fflush(stdout);
#endif

        renf_init(nf + i, p2, a, prec);
#ifdef DEBUG
        flint_printf("[renf_set_embeddings_fmpz_poly] renf_init done\n", i);
        fflush(stdout);
#endif

    }

#ifdef DEBUG
    printf("[renf_set_embeddings_fmpz_poly] cleaning...");
    fflush(stdout);
#endif
    arb_clear(a);
    fmpq_poly_clear(p2);
    _fmpz_vec_clear(c, n);
    flint_free(k);
#ifdef DEBUG
    printf("done\n");
    fflush(stdout);
#endif
    return n_interval;
}
