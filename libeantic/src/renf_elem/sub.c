/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"

void renf_elem_sub(renf_elem_t a, const renf_elem_t b, const renf_elem_t c, const renf_t nf)
{
    nf_elem_sub(a->elem, b->elem, c->elem, nf->nf);
    arb_sub(a->emb, b->emb, c->emb, nf->prec);
}
