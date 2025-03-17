/*
    Copyright (C) 2025 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/config.h"

#include "../../e-antic/renf_elem.h"

static void nf_elem_submul_ui(nf_elem_t res, const nf_elem_t a, const ulong b, const nf_t nf)
{
    nf_elem_t c;
    nf_elem_init(c, nf);

    fmpz_t bmpz;
    fmpz_init_set_ui(bmpz, b);

    nf_elem_scalar_mul_fmpz(c, a, bmpz, nf);
    nf_elem_sub(res, res, c, nf);

    fmpz_clear(bmpz);
    nf_elem_clear(c, nf);
}

void renf_elem_submul_ui(renf_elem_t res, const renf_elem_t a, const ulong b, const renf_t nf)
{
    nf_elem_submul_ui(res->elem, a->elem, b, nf->nf);
    arb_submul_ui(res->emb, a->emb, b, nf->prec);
}
