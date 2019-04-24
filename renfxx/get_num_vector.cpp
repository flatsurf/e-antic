/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "e-antic/renfxx.h"

std::vector<mpz_class> renf_elem_class::get_num_vector(void) const
{
    mpz_class x;
    std::vector<mpz_class> res;

    if (nf == NULL)
    {
        fmpz_get_mpz(x.__get_mp(), fmpq_numref(b));
        res.push_back(x);
    }
    else
    {
        fmpq_poly_t f;
        fmpq_poly_init(f);
        nf_elem_get_fmpq_poly(f, a->elem, nf->get_renf()->nf);
        for (size_t i = 0; i < fmpq_poly_length(f); i++)
        {
            fmpz_get_mpz(x.__get_mp(), fmpq_poly_numref(f) + i);
            res.push_back(x);
        }
        size_t deg = fmpq_poly_degree(nf->get_renf()->nf->pol);
        for (size_t i = fmpq_poly_length(f); i < deg; i++)
            res.push_back(mpz_class(0));
        fmpq_poly_clear(f);
    }

    return res;
}
