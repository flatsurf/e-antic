/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"
#include "../e-antic/fmpz_poly_extra.h"

slong renf_elem_get_cfrac(fmpz * c, renf_elem_t rem, renf_elem_t a, slong n, renf_t nf)
{
    slong i;

    renf_elem_set(rem, a, nf);
    if (renf_elem_is_zero(rem, nf)) return 0;

    for (i = 0; i < n; i++)
    {
        renf_elem_floor(c + i, rem, nf);
        renf_elem_sub_fmpz(rem, rem, c + i, nf);
        if (renf_elem_is_zero(rem, nf)) return i+1;
        renf_elem_inv(rem, rem, nf);
    }

    return n;
}
