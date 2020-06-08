/*
    Copyright (C) 2009 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gmp.h>
#include <e-antic/poly_extra.h>

void EANTIC_FIXED_fmpq_poly_add_fmpq(fmpq_poly_t res, const fmpq_poly_t poly, const fmpq_t c)
{
    fmpz_t d;

    if (poly -> length == 0)
    {
        fmpq_poly_set_fmpq(res, c);
        return;
    }

    fmpq_poly_set(res, poly);

    if (poly -> length == 1)
    {
        _fmpq_add(res->coeffs, res->den, poly->coeffs, poly->den, fmpq_numref(c), fmpq_denref(c));
        if (fmpz_is_zero(res->coeffs))
          res->length = 0;
        return;
    }

    if (fmpz_equal(res->den, fmpq_denref(c)))
    {
        fmpz_add(res->coeffs + 0, res->coeffs + 0, fmpq_numref(c));

        if (!fmpz_is_one(res->den))
        {
            fmpz_init(d);
            fmpz_gcd(d, res->den, res->coeffs + 0);

            if (!fmpz_is_one(d))
            {
                _fmpz_vec_content(d, res->coeffs, res->length);
                if (!fmpz_is_one(d))
                {
                    fmpz_gcd(d, d, res->den);
                    _fmpz_vec_scalar_divexact_fmpz(res->coeffs, res->coeffs, res->length, d);
                    fmpz_divexact(res->den, res->den, d);
                }
            }
            fmpz_clear(d);
        }
        return;
    }

    fmpz_init(d);
    fmpz_one(d);

    if (!fmpz_is_one(poly->den) && !fmpz_is_one(fmpq_denref(c)))
        fmpz_gcd(d, poly->den, fmpq_denref(c));

    if (fmpz_is_one(d))
    {
        _fmpz_vec_scalar_mul_fmpz(res->coeffs, res->coeffs, res->length, fmpq_denref(c));
        fmpz_addmul(res->coeffs, fmpq_numref(c), res->den);
        fmpz_mul(res->den, res->den, fmpq_denref(c));
    }
    else
    {
        fmpz_t den11;
        fmpz_t den22;
        fmpz_t e;

        fmpz_init(den11);
        fmpz_init(den22);
        fmpz_init(e);

        fmpz_divexact(den11, res->den, d);
        fmpz_divexact(den22, fmpq_denref(c), d);

        _fmpz_vec_scalar_mul_fmpz(res->coeffs, res->coeffs, res->length, den22);
        fmpz_addmul(res->coeffs, fmpq_numref(c), den11);
        _fmpz_vec_content(e, res->coeffs, res->length);
        if (!fmpz_is_one(e))
            fmpz_gcd(e, e, d);

        if (fmpz_is_one(e))
            fmpz_mul(res->den, res->den, den22);
        else
        {
            _fmpz_vec_scalar_divexact_fmpz(res->coeffs, res->coeffs, res->length, e);
            fmpz_divexact(den11, res->den, e);
            fmpz_mul(res->den, den11, den22);
        }
        fmpz_clear(e);
        fmpz_clear(den11);
        fmpz_clear(den22);
    }
    fmpz_clear(d);
}
