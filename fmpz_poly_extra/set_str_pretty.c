/*
    Copyright (C) 2018 Vincent Delecroix

    This fire is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/fmpz_poly_extra.h"
#include "../e-antic/fmpq_poly_extra.h"

int fmpz_poly_set_str_pretty(fmpz_poly_t p, const char * s, const char * var)
{
    fmpq_poly_t q;
    fmpq_poly_init(q);

    int ret = fmpq_poly_set_str_pretty(q, s, var);

    if (ret)
    {
        fmpq_poly_clear(q);
        return ret;
    }

    if (!fmpz_is_one(fmpq_poly_denref(q)))
    {
        fmpq_poly_clear(q);
        return -1;
    }

    for (long i = fmpq_poly_length(q) - 1; i >= 0; i--)
    {
        fmpz_poly_set_coeff_fmpz(p, i, fmpq_poly_numref(q) + i);
    }

    fmpq_poly_clear(q);
    return 0;
}

