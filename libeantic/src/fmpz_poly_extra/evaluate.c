/*
    Copyright (C) 2016 Vincent Delecroix
                  2023 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/fmpz_poly_extra.h"

void _fmpz_poly_evaluate_arb(arb_t res, const fmpz * pol, mp_limb_signed_t len, const arb_t a, mp_limb_signed_t prec)
{
    mp_limb_signed_t i;

    arb_zero(res);
    for (i = len - 1; i >= 0; i--)
    {
        arb_mul(res,res,a,prec);
        arb_add_fmpz(res,res,pol+i,prec);
    }
}

void fmpz_poly_evaluate_arb(arb_t res, const fmpz_poly_t pol, const arb_t a, mp_limb_signed_t prec)
{
    if (a == res)
    {
        arb_t tmp;
        arb_init(tmp);

        _fmpz_poly_evaluate_arb(tmp, pol->coeffs, fmpz_poly_length(pol), a, prec);
        arb_swap(tmp, res);

        arb_clear(tmp);
    }
    else
    {
      _fmpz_poly_evaluate_arb(res, pol->coeffs, fmpz_poly_length(pol), a, prec);
    }
}

void _fmpz_poly_evaluate_arf(arf_t res, const fmpz * pol, mp_limb_signed_t len, const arf_t a, mp_limb_signed_t prec)
{
    mp_limb_signed_t i;

    arf_zero(res);
    for (i = len - 1; i >= 0; i--)
    {
        arf_mul_rnd_any(res, res, a, prec, 4);
        arf_add_fmpz(res,res,pol+i,prec,4);
    }
}

void fmpz_poly_evaluate_arf(arf_t res, const fmpz_poly_t pol, const arf_t a, mp_limb_signed_t prec)
{
    if (a == res)
    {
        arf_t tmp;
        arf_init(tmp);

        _fmpz_poly_evaluate_arf(tmp, pol->coeffs, fmpz_poly_length(pol), a, prec);
        arf_swap(tmp, res);

        arf_clear(tmp);
    }
    else
    {
        _fmpz_poly_evaluate_arf(res, pol->coeffs, fmpz_poly_length(pol), a, prec);
    }
}
