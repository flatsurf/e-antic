/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "poly_extra.h"

int fmpq_poly_check_unique_complex_root(const fmpq_poly_t pol, const acb_t a, slong prec)
{
    acb_t b;
    fmpq_poly_t der;

    /* we check that:                        */
    /*  - there is a root                    */
    /*  - the derivative has no root         */

    acb_init(b);
    fmpq_poly_evaluate_acb(b, pol, a, prec);
    if (!acb_contains_zero(b))
    {
        acb_clear(b);
        return 0;
    }

    fmpq_poly_init(der);
    fmpq_poly_derivative(der, pol);
    fmpq_poly_evaluate_acb(b, der, a, prec);
    if (acb_contains_zero(b))
    {
        acb_clear(b);
        fmpq_poly_clear(der);
        return 0;
    }

    acb_clear(b);
    fmpq_poly_clear(der);
    return 1;
}
