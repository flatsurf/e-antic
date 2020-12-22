/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"

void renf_elem_set_ui(renf_elem_t a, ulong n, const renf_t nf)
{
    nf_elem_set_ui(a->elem, n, nf->nf);
    arb_set_ui(a->emb, n);
}
