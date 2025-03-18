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

static void nf_elem_submul_fmpq(nf_elem_t res, const nf_elem_t a, const fmpq_t b, const nf_t nf)
{
    nf_elem_t c;
    nf_elem_init(c, nf);

    nf_elem_scalar_mul_fmpq(c, a, b, nf);
    nf_elem_sub(res, res, c, nf);

    nf_elem_clear(c, nf);
}

static void arb_submul_fmpq(arb_t res, const arb_t a, const fmpq_t b, slong prec)
{
    arb_t b_arb;
    arb_init(b_arb);
    arb_set_fmpq(b_arb, b, prec);

    arb_submul(res, a, b_arb, prec);

    arb_clear(b_arb);
}

void renf_elem_submul_fmpq(renf_elem_t res, const renf_elem_t a, const fmpq_t b, const renf_t nf)
{
    nf_elem_submul_fmpq(res->elem, a->elem, b, nf->nf);
    arb_submul_fmpq(res->emb, a->emb, b, nf->prec);
}


