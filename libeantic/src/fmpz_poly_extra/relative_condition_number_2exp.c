/*
    Copyright (C) 2019 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/fmpz_poly_extra.h"

int _fmpz_poly_relative_condition_number_2exp(slong * cond, fmpz * p, slong len, arb_t x, slong prec)
{
    arf_t abs, res;
    slong i;
    fmpz_t cond2;

    arf_init(res);
    arf_init(abs);
    fmpz_init(cond2);

    arb_get_abs_ubound_arf(abs, x, prec);

    if (fmpz_sgn(p + len - 1) > 0)
        arf_set_round_fmpz(res, p + len - 1, prec, ARF_RND_UP);
    else
    {
        arf_set_round_fmpz(res, p + len - 1, prec, ARF_RND_DOWN);
        arf_neg(res, res);
    }
    for (i = len - 2; i >= 0; i--)
    {
        arf_mul(res, res, abs, prec, ARF_RND_UP);
        if (fmpz_sgn(p + i) > 0)
            arf_add_fmpz(res, res, p + i, prec, ARF_RND_UP);
        else if (fmpz_sgn(p + i) < 0)
            arf_sub_fmpz(res, res, p + i, prec, ARF_RND_UP);
    }
    arf_abs_bound_lt_2exp_fmpz(cond2, res);
    arf_clear(res);
    arf_clear(abs);

    if (!fmpz_fits_si(cond2))
    {
        fmpz_clear(cond2);
        return 1;
    }
    *cond = fmpz_get_si(cond2);
    fmpz_clear(cond2);
    return 0;
}

int fmpz_poly_relative_condition_number_2exp(slong * cond, fmpz_poly_t p, arb_t x, slong prec)
{
    return _fmpz_poly_relative_condition_number_2exp(cond, p->coeffs, p->length, x, prec);
}

