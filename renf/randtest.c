/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "renf.h"
#include "poly_extra.h"

void renf_randtest(renf_t nf, flint_rand_t state, slong len, mp_bitcnt_t bits)
{
    fmpz_poly_t p;

    if (len < 2)
    {
        fprintf(stderr, "ERROR (renf_randtest): got length < 2\n");
        flint_abort();
    }

    do{
        fmpz_poly_randtest_irreducible(p, state, len, bits);
    }while((fmpz_poly_length(p) < 2) || !fmpz_poly_has_real_root(p));

}
