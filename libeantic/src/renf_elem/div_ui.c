/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"

void renf_elem_div_ui(renf_elem_t a, const renf_elem_t b, ulong c, const renf_t nf)
{
    fmpz_t tmp;
    fmpz_init(tmp);
    fmpz_set_ui(tmp, c);
    nf_elem_scalar_div_fmpz(a->elem, b->elem, tmp, nf->nf);
    arb_div_fmpz(a->emb, b->emb, tmp, nf->prec);
    fmpz_clear(tmp);
}
