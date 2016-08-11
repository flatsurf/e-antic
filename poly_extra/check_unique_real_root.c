/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "poly_extra.h"

int fmpq_poly_check_unique_real_root(const fmpq_poly_t pol, const arb_t a, slong prec)
{
    arb_t b;
    fmpq_poly_t der;

    /* we check that:                        */
    /*  - there is a root                    */
    /*  - the derivative has no root         */
    /*printf("q="); fmpq_poly_print(pol); printf("\n");*/

    arb_init(b);
    fmpq_poly_evaluate_arb(b, pol, a, prec);
    /*printf("a="); arb_print(a); printf("\n");*/
    if (!arb_contains_zero(b))
    {
        arb_clear(b);
        return 0;
    }

    fmpq_poly_init(der);
    fmpq_poly_derivative(der, pol);
    fmpq_poly_evaluate_arb(b, der, a, prec);
    /*printf("der="); fmpq_poly_print(der); printf("\n");*/
    /*printf("b="); arb_print(b); printf("\n");*/
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
