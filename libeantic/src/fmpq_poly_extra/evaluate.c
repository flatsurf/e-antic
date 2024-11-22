/*
    Copyright (C) 2016 Vincent Delecroix
                  2023 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/config.h"

#include "../../e-antic/fmpq_poly_extra.h"
#include "../../e-antic/fmpz_poly_extra.h"

static void _fmpq_poly_evaluate_arf(arf_t res, const fmpq_poly_t pol, const arf_t a, mp_limb_signed_t prec)
{
    _fmpz_poly_evaluate_arf(res, ((pol)->coeffs), fmpq_poly_length(pol), a, prec);
    arf_div_fmpz(res, res, ((pol)->den), prec, 4);
}

void fmpq_poly_evaluate_arf(arf_t res, const fmpq_poly_t pol, const arf_t a, mp_limb_signed_t prec)
{
    if (a == res)
    {
        arf_t tmp;
        arf_init(tmp);

        _fmpq_poly_evaluate_arf(tmp, pol, a, prec);
        arf_swap(tmp, res);

        arf_clear(tmp);
    }
    else
    {
        _fmpq_poly_evaluate_arf(res, pol, a, prec);
    }
}

static void _fmpq_poly_evaluate_arb(arb_t res, const fmpq_poly_t pol, const arb_t a, mp_limb_signed_t prec)
{
    _fmpz_poly_evaluate_arb(res, ((pol)->coeffs), fmpq_poly_length(pol), a, prec);
    arb_div_fmpz(res, res, ((pol)->den), prec);
}

void fmpq_poly_evaluate_arb(arb_t res, const fmpq_poly_t pol, const arb_t a, mp_limb_signed_t prec)
{
    if (a == res)
    {
        arb_t tmp;
        arb_init(tmp);

        _fmpq_poly_evaluate_arb(tmp, pol, a, prec);
        arb_swap(tmp, res);

        arb_clear(tmp);
    }
    else
    {
        _fmpq_poly_evaluate_arb(res, pol, a, prec);
    }
}


