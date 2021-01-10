/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf.h"

void renf_print(const renf_t nf)
{
    flint_printf("NumberField(");
    fmpq_poly_print_pretty(nf->nf->pol, "x");
    flint_printf(", ");
    arb_print(nf->emb);
    flint_printf(")");
}
