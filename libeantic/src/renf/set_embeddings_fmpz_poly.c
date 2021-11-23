/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <assert.h>

#include "../e-antic/renf.h"
#include "../e-antic/fmpz_poly_extra.h"

slong renf_set_embeddings_fmpz_poly(renf * nf, fmpz_poly_t pol, slong lim, slong prec)
{
    slong i, n, n_exact, n_interval;
    fmpq_poly_t p2;
    arb_t a;
    fmpz * c;
    slong * k;

    n = fmpz_poly_num_real_roots_upper_bound(pol);
    c = _fmpz_vec_init(n);

    assert(n >= 0);
    k = (slong *) flint_malloc((ulong)n * sizeof(slong));

    fmpz_poly_isolate_real_roots(NULL, &n_exact, c, k, &n_interval, pol);

    if (n_exact)
    {
        fprintf(stderr, "ERROR (fmpz_poly_real_embeddings): rational roots\n");
        abort();
    }

    arb_init(a);
    fmpq_poly_init(p2);
    fmpz_one(fmpq_poly_denref(p2));
    fmpq_poly_fit_length(p2, pol->length);
    _fmpz_vec_set(p2->coeffs, pol->coeffs, pol->length);
    p2->length = pol->length;
    for (i = 0; i < FLINT_MIN(lim, n_interval); i++)
    {
        arb_set_fmpz(a, c + i);
        arb_mul_2exp_si(a, a, 1);
        arb_add_si(a, a, 1, prec);
        mag_one(arb_radref(a));
        arb_mul_2exp_si(a, a, k[i] - 1);

        renf_init(nf + i, p2, a, prec);

    }

    arb_clear(a);
    fmpq_poly_clear(p2);
    _fmpz_vec_clear(c, n);
    flint_free(k);

    return n_interval;
}

