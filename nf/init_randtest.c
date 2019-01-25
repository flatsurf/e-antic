/*=============================================================================

    This file is part of ANTIC.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2019 Vincent Delecroix

******************************************************************************/

#include "e-antic/poly_extra.h"
#include "e-antic/nf.h"

void nf_init_randtest(nf_t nf, flint_rand_t state,
        slong len,
        mp_bitcnt_t bits_in)
{
    fmpq_poly_t pol;
    fmpz_poly_t q;

    if (len < 2 || bits_in < 1)
    {
        fprintf(stderr, "[nf_init_randtest] len must be >= 2 and bits_in >= 1\n");
        abort();
    }

    if (len <= 2 || n_randint(state, 30) == 0)
        len = 2; /* linear */
    else if (len <= 3 || n_randint(state, 30) == 0)
        len = 3; /* quadratic */
    else
        len = 3 + n_randint(state, len-2);

    fmpz_poly_init(q);
    do {
        fmpz_poly_randtest(q,
                state,
                len,
                1 + n_randint(state, bits_in));
    } while (fmpz_poly_degree(q) < 1 || fmpz_is_zero(q->coeffs));

    fmpq_poly_init(pol);
    fmpq_poly_set_fmpz_poly(pol, q);

    if (n_randint(state, 5) == 0)
        fmpz_one(pol->coeffs + pol->length - 1); /* monic */
    else
        fmpz_randtest_not_zero(fmpq_poly_denref(pol), state, bits_in);
    fmpq_poly_canonicalise(pol);

    nf_init(nf, pol);
    fmpq_poly_clear(pol);
    fmpz_poly_clear(q);
}
