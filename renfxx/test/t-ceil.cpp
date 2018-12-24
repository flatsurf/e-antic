/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/renfxx.h"

int main(void)
{
    FLINT_TEST_INIT(state);
    int iter;

    {
        fmpq_t x;

        fmpq_init(x);
        fmpz_set_si(fmpq_numref(x), 3);
        fmpz_set_si(fmpq_denref(x), 2);

        renf_elem_class a(x);

        if (a.ceil() != 2)
            throw std::runtime_error("pb with ceil");

        fmpz_set_si(fmpq_numref(x), -3);
        fmpz_set_si(fmpq_denref(x), 2);

        a = x;

        if (a.ceil() != -1)
            throw std::runtime_error("pb with ceil");

        fmpq_clear(x);
    }

    {
        renf_t nf;
        arb_t emb;
        fmpq_t k;
        fmpq_poly_t p;

        fmpq_init(k);
        fmpq_poly_init(p);

        fmpq_poly_set_coeff_si(p, 2, 1);
        fmpq_poly_set_coeff_si(p, 1, -1);
        fmpq_poly_set_coeff_si(p, 0, -1);

        arb_init(emb);
        arb_set_d(emb, 1.61803398874989);
        arb_add_error_2exp_si(emb, -20);
        renf_init(nf, p, emb, 20 + n_randint(state, 100));
        arb_clear(emb);

        renf_class K(nf);
        renf_clear(nf);

        /* (1+sqrt(5))/2 vs Fibonacci */
        renf_elem_class a(K);
        fmpq_poly_zero(p);
        fmpq_poly_set_coeff_si(p, 1, -1);
        for (iter = 1; iter < 2000; iter++)
        {
            fmpz_fib_ui(fmpq_numref(k), iter+1);
            fmpz_fib_ui(fmpq_denref(k), iter);
            fmpq_poly_set_coeff_fmpq(p, 0, k);
            renf_elem_set_fmpq_poly(a.get_renf_elem(), p, nf);

            if (a.ceil() != 1 - iter % 2)
                throw std::runtime_error("pb with Fibonacci ceil");
        }

        fmpq_clear(k);
        fmpq_poly_clear(p);
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}

