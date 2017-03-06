/*
    Copyright (C) 2015, Elias Tsigaridas
    Copyright (C) 2016, Vincent Delecroix

    This file is part of e-antic.

    The implementation follows the SLV library version 0.5 by Elias Tsigaridas
    (namely from the file poly_ops.c lines 119-139).

    (namely the function Descartes_test in the file vca_solver_1.c lines
    67-125)

    e-atnci is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/poly_extra.h>

/* the function below returns the sign of pol(1/2) */
/* TODO: avoid allocation of x and y               */
int _fmpz_poly_sgn_at_half(fmpz * pol, slong len)
{
    slong j,k;
    int ret;
    fmpz_t x, y;

    fmpz_init(x);
    fmpz_init(y);
    fmpz_zero(y);

    k = len;
    for (j = 0; j <= len - 1; j++, k--)
    {
        fmpz_mul_2exp(y, pol + j, k);
        fmpz_add(x, x, y);
    }

    ret = fmpz_sgn(x);
    fmpz_clear(x);
    fmpz_clear(y);
    return ret;
}

