/*
    Copyright (C) 2019 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <flint/flint.h>
#include <e-antic/renf.h>
#include <e-antic/renf_elem.h>

#define NB_THREADS 4
#define CFRAC_LENGTH 300

typedef struct {
    renf_ptr nf;
    int err;
} thread_data;
void * thread_fcn(void *);

int main(void)
{
    pthread_t t[NB_THREADS];
    int i, result;

    fmpq_poly_t p;
    arb_t emb;
    renf_t nf;
    fmpq_poly_init(p);
    arb_init(emb);
    fmpq_poly_set_str_pretty(p, "x^4-100*x^3+50*x^2-20*x+1", "x");
    arb_set_d(emb, 0.057258181166);
    arb_add_error_2exp_si(emb, -12);
    renf_init(nf, p, emb, 128);
    fmpq_poly_clear(p);
    arb_clear(emb);

    for (i = 0; i < NB_THREADS; i++)
    {
        if (pthread_create( &t[i], NULL, thread_fcn, (void *) nf) != 0)
        {
            err(1, "not able to create thread %d\n", i);
        }
    }

    for (i = 0; i < NB_THREADS; i++)
    {
        pthread_join(t[i], (void **) &result);
        if (result)
        {
            err(1, "wrong behavior in thread %d\n", i);
        }
    }

    exit(EXIT_SUCCESS);
}


/* the function below is run in each thread. It computes the continued fraction */
/* of 10 random elements in the number field passed in argument.                */
void * thread_fcn(void * ptr)
{
    renf_ptr nf = (renf_ptr) ptr;
    renf_elem_t x, rem;
    int j;
    fmpz * cf1 = _fmpz_vec_init(CFRAC_LENGTH);
    fmpz * cf2 = _fmpz_vec_init(CFRAC_LENGTH);
    FLINT_TEST_INIT(state);

    renf_elem_init(x, nf);
    renf_elem_init(rem, nf);
    for (j = 0; j < 10; j++)
    {
        do
        {
            renf_elem_randtest(x, state, 10, nf);
        } while (renf_elem_is_zero(x, nf));
        renf_elem_get_cfrac(cf1, rem, x, CFRAC_LENGTH, nf);
        renf_elem_get_cfrac(cf2, rem, x, CFRAC_LENGTH, nf);
        if (! _fmpz_vec_equal(cf1, cf2, CFRAC_LENGTH-1))
            pthread_exit((void *) 1);
    }

    _fmpz_vec_clear(cf1, CFRAC_LENGTH);
    _fmpz_vec_clear(cf2, CFRAC_LENGTH);
    renf_elem_clear(x, nf);
    renf_elem_clear(rem, nf);
    FLINT_TEST_CLEANUP(state);

    pthread_exit((void *) 0);
}
