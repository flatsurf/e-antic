/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf.h"

void renf_clear(renf_t nf)
{
    nf_clear(nf->nf);
    fmpz_poly_clear(nf->der);
    arb_clear(nf->emb);
}
