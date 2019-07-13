/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <err.h>
#include <e-antic/poly_extra.h>
#include <e-antic/renf.h>

#include "../config.h"

#ifdef HAVE_PTHREAD
#include <pthread.h>
static pthread_once_t mtx_initialized;
static pthread_mutex_t mtx;

void initialize_mtx()
{
    if (pthread_mutex_init(&mtx, NULL) != 0)
    {
	err(1, "failed to create mutex for renf_refine_embedding()");
    }
}
#endif

void renf_refine_embedding(renf_t nf, slong prec)
{
#ifdef HAVE_PTHREAD
    pthread_once(&mtx_initialized, initialize_mtx);

    // We need to make sure that no two threads attempt to refine the same
    // number field embedding at the same time: if two threads were looking for
    // different precisions, we might end up with the lower precision
    // eventually. Also the arb_swap() would not be safe.
    if (pthread_mutex_lock(&mtx) != 0)
    {
	err(1, "failed to lock mutex for renf_refine_embedding()");
    }
#endif

    arb_t tmp;
    slong cond;
    slong comp_prec;

    _fmpz_poly_relative_condition_number_2exp(&cond,
            nf->nf->pol->coeffs, nf->nf->pol->length,
            nf->emb, FLINT_MAX(nf->prec, 64));
    cond *= nf->nf->pol->length;
    arb_init(tmp);

    comp_prec = 2 * FLINT_ABS(arb_rel_accuracy_bits(nf->emb)) + cond;
    while (arb_rel_accuracy_bits(nf->emb) < prec)
    {
        if(!_fmpz_poly_newton_step_arb(tmp,
                fmpq_poly_numref(nf->nf->pol),
                nf->der->coeffs,
                fmpq_poly_length(nf->nf->pol),
                nf->emb,
                comp_prec))
        {
            if(!_fmpz_poly_bisection_step_arb(tmp,
                    fmpq_poly_numref(nf->nf->pol),
                    fmpq_poly_length(nf->nf->pol),
                    nf->emb,
                    comp_prec))
            {
                comp_prec *= 2;
                continue;
            }
        }
        arb_swap(tmp, nf->emb);

        _fmpz_poly_relative_condition_number_2exp(&cond,
                nf->nf->pol->coeffs, nf->nf->pol->length,
                nf->emb, 32);
        comp_prec = 2 * FLINT_ABS(arb_rel_accuracy_bits(nf->emb)) + cond * nf->nf->pol->length;
    }
    arb_clear(tmp);

#ifdef HAVE_PTHREAD
    if (pthread_mutex_unlock(&mtx) != 0)
    {
	err(1, "failed to unlock mutex for renf_refine_embedding()");
    }
#endif
}
