/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/config.h"

#include "../../e-antic/renf.h"
#include "../../e-antic/renf_elem.h"

int main(void)
{
    int i;

    FLINT_TEST_INIT(state);

    printf("init_nth_root_fmpq....");
    
    for(i = 0; i < 10; i++)
    {
        renf_t nf;
        renf_elem_t a, b;
        fmpq_t d;
        fmpq_poly_t p;

        fmpq_init(d);
        fmpq_poly_init(p);
        fmpq_set_si(d, (slong)n_randtest_not_zero(state), 37);
        if (fmpq_cmp_si(d, 0) < 0)
            fmpq_neg(d, d);
        renf_init_nth_root_fmpq(nf, d, 5, 64);

        renf_elem_init(a, nf);
        renf_elem_init(b, nf);

        fmpq_poly_set_coeff_si(p, 1, 1);
        renf_elem_set_fmpq_poly(a, p, nf);

        renf_elem_mul(b, a, a, nf);
        renf_elem_mul(b, b, b, nf);
        renf_elem_mul(b, b, a, nf);

        fmpq_clear(d);
        renf_elem_clear(a, nf);
        renf_elem_clear(b, nf);
        renf_clear(nf);
        fmpq_poly_clear(p);
    }

#ifdef FLINT_TEST_CLEAR
    FLINT_TEST_CLEAR(state)
#else
    // FLINT <3.2.2 calls this macro FLINT_TEST_CLEAR instead of FLINT_TEST_CLEANUP
    FLINT_TEST_CLEANUP(state)
#endif

    printf("PASS\n");
    return 0;
}
