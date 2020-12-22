/*
    Copyright (C) 2015, Elias Tsigaridas
    Copyright (C) 2016, Vincent Delecroix

    This file is part of e-antic.

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/fmpz_poly_extra.h"

slong fmpz_poly_num_real_roots_upper_bound(fmpz_poly_t pol)
{
    slong i, i0, k, ret, len;
    fmpz * pol2;

    if (fmpz_poly_is_zero(pol))
    {
        fprintf(stderr, "ERROR (fmpz_poly_positive_roots_upper_bound): zero polynomial\n");
        abort();
    }

    i0 = 0;
    while (fmpz_is_zero(pol->coeffs + i0))
        i0++;
    len = pol->length - i0;
    pol2 = _fmpz_vec_init(len);

    /* zero roots */
    ret = i0;

    /* positive roots */
    _fmpz_vec_set(pol2, pol->coeffs + i0, len);

    k = _fmpz_poly_positive_root_upper_bound_2exp(pol2, len);
    if (k != WORD_MIN)
    {
        _fmpz_poly_scale_2exp(pol2, len, k);
        ret += _fmpz_poly_descartes_bound_0_1(pol2, len, len);
    }

    /* negative roots */
    _fmpz_vec_set(pol2, pol->coeffs + i0, len);
    for (i = 1; i < len; i += 2)
        fmpz_neg(pol2 + i, pol2 + i);

    k = _fmpz_poly_positive_root_upper_bound_2exp(pol2, len);
    if (k != WORD_MIN)
    {
        _fmpz_poly_scale_2exp(pol2, len, k);
        ret += _fmpz_poly_descartes_bound_0_1(pol2, len, len);
    }

    _fmpz_vec_clear(pol2, len);
    return ret;
}

