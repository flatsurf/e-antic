/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"

int renf_elem_cmp_fmpz(renf_elem_t a, const fmpz_t b, renf_t nf)
{
    int s;
    fmpq_t bq;

    fmpq_init(bq);
    fmpz_set(fmpq_numref(bq), b);  /* TODO: avoid copy! */
    fmpz_one(fmpq_denref(bq));

    s = renf_elem_cmp_fmpq(a, bq, nf);
    fmpq_clear(bq);
    return s;
}
