/*
    Copyright (C) 2018 Vincent Delecroix
    Copyright (C) 2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renfxx.h>

mpz_class renf_elem_class::num_content() const noexcept {
    mpz_class x;

    if (!nf)
    {
        fmpz_get_mpz(x.__get_mp(), fmpq_numref(b));
        if (fmpq_sgn(b) < 0)
            mpz_neg(x.__get_mp(), x.__get_mp());
    }
    else if (nf->get_renf()->nf->flag & NF_LINEAR)
    {
        fmpz_get_mpz(x.__get_mp(), LNF_ELEM_NUMREF(a->elem));
        if (fmpz_sgn(LNF_ELEM_NUMREF(a->elem)) < 0)
            mpz_neg(x.__get_mp(), x.__get_mp());
    }
    else if (nf->get_renf()->nf->flag & NF_QUADRATIC)
    {
        fmpz_t t;
        fmpz_init(t);
        fmpz_gcd(t, QNF_ELEM_NUMREF(a->elem), QNF_ELEM_NUMREF(a->elem) + 1);
        fmpz_get_mpz(x.__get_mp(), t);
        fmpz_clear(t);
    }
    else
    {
        fmpz_t t;
        fmpz_init(t);
        _fmpz_poly_content(t, NF_ELEM(a->elem)->coeffs, NF_ELEM(a->elem)->length);
        fmpz_get_mpz(x.__get_mp(), t);
        fmpz_clear(t);
    }

    return x;
}
