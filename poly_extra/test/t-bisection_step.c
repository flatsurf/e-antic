/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "flint.h"
#include "arb.h"
#include "poly_extra.h"

int main(void)
{
    int ans;
    slong prec;
    fmpz_poly_t p;
    arb_t a,b;
    FLINT_TEST_INIT(state);

    printf("bisection_step....");


    fmpz_poly_init(p);
    arb_init(a);
    arb_init(b);

    fmpz_poly_set_coeff_si(p, 0, -2);
    fmpz_poly_set_coeff_si(p, 2, 1);

    arb_set_d(a, 1.40);
    arb_add_error_2exp_si(a, -5);

    for (prec = 50; prec < 1000; prec *= 2)
    {
        ans = fmpz_poly_bisection_step_arb(a, p, a, prec);
    }
    ans = fmpz_poly_bisection_step_arb(a, p, a, 50);

    fmpz_poly_clear(p);
    arb_clear(a);
    arb_clear(b);
    FLINT_TEST_CLEANUP(state);

    printf("PASS\n");
    return 0;
}
