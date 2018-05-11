/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renf_elem.h>


int nf_elem_is_integer(const nf_elem_t a, const nf_t nf)
{
    if (nf->flag & NF_LINEAR)
        return fmpz_is_one(LNF_ELEM_DENREF(a));
    else if (nf->flag & NF_QUADRATIC)
        return fmpz_is_zero(QNF_ELEM_NUMREF(a) + 1) &&
               fmpz_is_one(QNF_ELEM_DENREF(a));
    else
        return NF_ELEM(a)->length <= 1 && fmpz_is_one(NF_ELEM_DENREF(a));
}

int nf_elem_is_rational(const nf_elem_t a, const nf_t nf)
{
    if (nf->flag & NF_LINEAR) return 1;
    else if (nf->flag & NF_QUADRATIC)
        return fmpz_is_zero(QNF_ELEM_NUMREF(a) + 1);
    else
        return NF_ELEM(a)->length <= 1;
}
