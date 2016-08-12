/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "poly_extra.h"

static __inline__ int
arb_sgn2(arb_t a)
{
    if (arb_is_positive(a)) return 1;
    else if (arb_is_negative(a)) return -1;
    return 0;
}

int fmpq_poly_check_unique_real_root(const fmpq_poly_t pol, const arb_t a, slong prec)
{
    arb_t b,c;
    arf_t l,r;
    fmpq_poly_t der;
    int lsign, rsign;

    /* we check that:                        */
    /*  - the left side and right side of the ball evaluates to number with different signs                    */
    /*  - the derivative has no root         */
    /*printf("q="); fmpq_poly_print(pol); printf("\n");*/

    arb_init(b);
    arb_init(c);
    arf_init(l);
    arf_init(r);
    arb_get_interval_arf(l, r, a, prec);
#ifdef DEBUG
    printf("[fmpq_poly_check_unique_real_root] l = "); arf_printd(l, 20); printf("\n");
    printf("[fmpq_poly_check_unique_real_root] r = "); arf_printd(r, 20); printf("\n");
#endif

    arb_set_arf(b, l);
    fmpq_poly_evaluate_arb(c, pol, b, 2*prec);
    lsign = arb_sgn2(c);

    arb_set_arf(b, r);
    fmpq_poly_evaluate_arb(c, pol, b, 2*prec);
    rsign = arb_sgn2(c);

    arb_clear(c);
    arf_clear(l);
    arf_clear(r);

    if (lsign * rsign != -1)
    {
        arb_clear(b);
        return 0;
    }

    fmpq_poly_init(der);
    fmpq_poly_derivative(der, pol);
    fmpq_poly_evaluate_arb(b, der, a, prec);

    if (arb_contains_zero(b))
    {
        arb_clear(b);
        fmpq_poly_clear(der);
        return 0;
    }

    arb_clear(b);
    fmpq_poly_clear(der);

    return 1;
}
