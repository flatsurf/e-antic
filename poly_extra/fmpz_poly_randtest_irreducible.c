/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"
#include "flint.h"
#include "fmpz_mod_poly.h"

void fmpz_poly_randtest_irreducible(fmpz_poly_t p, flint_rand_t state, slong len, mp_bitcnt_t bits)
{
    fmpz_t c;
    fmpz_mod_poly_t q;

    fmpz_init(c);

    fmpz_randprime(c, state, bits, 1);
    fmpz_mod_poly_init(q, c);
    fmpz_mod_poly_randtest_irreducible(q, state, len);
    fmpz_mod_poly_get_fmpz_poly(p, q);

    fmpz_mod_poly_clear(q);
    fmpz_clear(c);
}


