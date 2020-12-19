/*
    Copyright (C) 2019 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <flint/flint.h>

#include "../../e-antic/renf.h"
#include "../../e-antic/renf_elem.h"

int main(void)
{
    FLINT_TEST_INIT(state);
    fmpq_t k;
    fmpq_poly_t p;
    arb_t emb;
    renf_t nf;
    renf_elem_t a;

    fmpq_init(k);
    fmpq_poly_init(p);

    fmpq_poly_set_coeff_si(p, 2, 1);
    fmpq_poly_set_coeff_si(p, 1, -1);
    fmpq_poly_set_coeff_si(p, 0, -1);

    arb_init(emb);
    arb_set_d(emb, 1.61803398874989);
    arb_add_error_2exp_si(emb, -20);
    renf_init(nf, p, emb, 20 + (slong)n_randint(state, 10));
    arb_clear(emb);

    {
        arb_t x;
        fmpq_poly_t q;
        arb_init(x);

        fmpq_poly_init(q);
        fmpq_poly_set_coeff_si(q, 0, -2501);
        fmpq_poly_set_coeff_si(q, 1, 137);

        renf_elem_init(a, nf);
        renf_elem_set_fmpq_poly(a, q, nf);

        slong prec = 30;
        renf_elem_get_arb(x, a, nf, prec);
        if (prec > -arb_rel_error_bits(x))
            return 1;

        prec = 135;
        renf_elem_get_arb(x, a, nf, prec);
        if (prec > arb_rel_accuracy_bits(x))
            return 1;

        prec = 100;
        renf_elem_get_arb(x, a, nf, prec);
        if (prec > arb_rel_accuracy_bits(x))
            return 1;

        prec = 1000;
        renf_elem_get_arb(x, a, nf, prec);
        if (prec > arb_rel_accuracy_bits(x))
            return 1;

        fmpq_poly_clear(q);
        arb_clear(x);
        renf_elem_clear(a, nf);
    }
    renf_clear(nf);
    FLINT_TEST_CLEANUP(state)

    return 0;
}
