/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/fmpz_poly_extra.h"

void fmpz_poly_squarefree_part(fmpz_poly_t res, fmpz_poly_t poly)
{

    if (poly->length == 0)
        fmpz_poly_zero(res);
    else if (poly->length == 1)
    {
        fmpz_poly_one(res);
    }
    else
    {
        fmpz_poly_t der, gcd;

        fmpz_poly_init(der);
        fmpz_poly_init(gcd);

        fmpz_poly_derivative(der, poly);
        fmpz_poly_gcd(gcd, poly, der);
        fmpz_poly_div(res, poly, gcd);

        fmpz_poly_clear(der);
        fmpz_poly_clear(gcd);

        if (res->length && fmpz_cmp_ui(res->coeffs + res->length - 1, 0) < 0)
        {
            fmpz_poly_neg(res, res);
        }
    }
}
