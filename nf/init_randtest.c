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

#include "e-antic/nf.h"

void nf_init_randtest(nf_t nf, flint_rand_t state,
        slong len,
        mp_bitcnt_t bits_in)
{
    fmpq_poly_t pol;

    if (len < 2 || bits_in < 1)
    {
        fprintf(stderr, "[nf_init_randtest] len must be >= 2 and bits_in >= 1\n");
        abort();
    }

    fmpq_poly_init(pol);
    do {
       fmpq_poly_randtest_not_zero(pol, state, 2 + n_randint(state, len-1), 1 + n_randint(state, bits_in));
    } while (fmpq_poly_degree(pol) < 1);

    if (n_randint(state, 10) < 3)
    {
        /* make it monic */
        fmpz_one(pol->coeffs + pol->length - 1);
        fmpz_one(pol->den);
    }

    nf_init(nf, pol);
    fmpq_poly_clear(pol);
}
