/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"

void renf_elem_add_si(renf_elem_t a, const renf_elem_t b, slong c, const renf_t nf)
{
    nf_elem_add_si(a->elem, b->elem, c, nf->nf);
    arb_add_si(a->emb, b->emb, c, nf->prec);
}
