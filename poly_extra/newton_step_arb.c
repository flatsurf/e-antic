/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

int _fmpz_poly_newton_step_arb(arb_t res, const fmpz * pol, const fmpz * der, slong len, arb_t a, slong prec)
{
    arf_t m;
    arb_t arbm;

#ifdef DEBUG
    flint_printf("[_fmpz_poly_newton_step]: len = %wd\n", len);
    flint_printf("[_fmpz_poly_newton_step]: prec = %wd\n", prec);
    flint_printf("[_fmpz_poly_newton_step]: a = "); arb_printd(a,10); printf("\n");
#endif

    _fmpz_poly_evaluate_arb(res, der, len - 1, a, prec);
    if (arb_contains_zero(res)) return 0;

    arf_init(m);
    arb_init(arbm);
    _fmpz_poly_evaluate_arf(m, pol, len, arb_midref(a), prec);

#ifdef DEBUG
    printf("[_fmpz_poly_newton_step]: after poly_evaluate m = "); arf_printd(m,10); printf("\n");
    printf("[_fmpz_poly_newton_step]: after poly_evaluate res = "); arb_printd(res,10); printf("\n");
#endif

    arb_set_arf(arbm, m);
    arb_div(res, arbm, res, prec);
    arb_sub_arf(res, res, arb_midref(a), prec);
    arb_neg(res, res);

#ifdef DEBUG
    printf("[_fmpz_poly_newton_step]: after newton step res = "); arb_printd(res,10); printf("\n");
#endif

    arf_clear(m);
    arb_clear(arbm);

    return arb_contains(a, res) && !arb_equal(a, res);
}

int fmpz_poly_newton_step_arb(arb_t res, const fmpz_poly_t pol, const fmpz_poly_t der, arb_t a, slong prec)

{
    arb_t rres;
    int ans;

    if (a == res) arb_init(rres);
    else arb_swap(rres, res);

    ans = _fmpz_poly_newton_step_arb(rres, pol->coeffs, der->coeffs, fmpz_poly_length(pol), a, prec);

    arb_swap(rres, res);
    if (a == res) arb_clear(rres);

    return ans;
}
