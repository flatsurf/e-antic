/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"

void renf_elem_fmpq_div(renf_elem_t a, const fmpq_t b, const renf_elem_t c, const renf_t nf)
{
    renf_elem_set(a, c, nf);
    renf_elem_inv(a, a, nf);
    renf_elem_mul_fmpq(a, c, b, nf);
}
