/*
   Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/fmpz_poly_extra.h"

static void _fmpz_vec_abs(fmpz * res, fmpz * p, slong len)
{
    slong i;
    for (i = 0; i < len; i++)
        fmpz_abs(res + i, p + i);
}

void fmpz_poly_abs(fmpz_poly_t res, fmpz_poly_t p)
{
    slong len = fmpz_poly_length(p);

    if (res != p)
    {
        fmpz_poly_fit_length(res, len);
        _fmpz_poly_set_length(res, len);
    }
    _fmpz_vec_abs(res->coeffs, p->coeffs, len);
}
