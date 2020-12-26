/*
    Copyright (C) 2016-2019 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>

#include "../e-antic/fmpz_poly_extra.h"

int _fmpz_poly_newton_step_arb(arb_t res, const fmpz * pol, const fmpz * der, slong len, arb_t a, slong prec)
{
    /* Newton step corresponds to set: res = a - f(a) / f'(a)            */
    /* In order to make it works with intervals, we do the following     */
    /*   - set m to be the ball with radius 0 and same center as a       */
    /*   - set res = m - f(m) / f'(a) where evaluation of both f and f'  */
    /*     is done with ball evaluation                                  */
    arb_t m, y;

    if (prec <= 0)
    {
        fprintf(stderr, "invalid precision %ld\n", prec);
        abort();
    }

    _fmpz_poly_evaluate_arb(res, der, len - 1, a, prec);
    if (arb_contains_zero(res)) return 0;

    arb_init(m);
    arb_init(y);
    arf_set(arb_midref(m), arb_midref(a));
    mag_zero(arb_radref(m));

    _fmpz_poly_evaluate_arb(y, pol, len, m, prec);

    arb_div(res, y, res, prec);
    arb_sub(res, res, m, prec);
    arb_neg(res, res);

    arb_clear(m);
    arb_clear(y);

    return arb_contains(a, res) && !arb_equal(a, res);
}

int fmpz_poly_newton_step_arb(arb_t res, const fmpz_poly_t pol, const fmpz_poly_t der, arb_t a, slong prec)

{
    return _fmpz_poly_newton_step_arb(res, pol->coeffs, der->coeffs, fmpz_poly_length(pol), a, prec);
}
