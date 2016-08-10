/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "nf_emb.h"
#include "fmpq.h"

void nf_emb_init_nth_root_fmpq(nf_emb_t nf, fmpq_t d, ulong n)
{
	fmpq_t p0;
	fmpq_poly_t pol;
	arb_t emb;

	fmpq_poly_init(pol);
	fmpq_init(p0);
	fmpq_set(p0, d);
	fmpq_neg(p0, p0);
	fmpq_poly_set_coeff_fmpq(pol, 0, p0);
	fmpq_poly_set_coeff_ui(pol, n, 1);

	arb_init(emb);
	arb_set_fmpq(emb, d, DEFAULT_PREC);
	arb_root_ui(emb, emb, n, DEFAULT_PREC);

	nf_emb_init(nf, pol, emb);

	fmpq_clear(p0);
	fmpq_poly_clear(pol);
	arb_clear(emb);
}

