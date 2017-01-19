/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "renf_elem.h"

int _nf_elem_is_rational(const nf_elem_t a, const nf_t nf)
{
    if (nf->flag & NF_LINEAR) return 1;
    else if (nf->flag & NF_QUADRATIC)
        return fmpz_is_zero(QNF_ELEM_NUMREF(a) + 1);
    else
        return NF_ELEM(a)->length <= 1;
}
