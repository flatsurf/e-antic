/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of nf_emb

    nf_emb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "fmpq_poly_extra.h"

void fmpq_poly_evaluate_arb(arb_t res, const fmpq_poly_t pol, const arb_t a, slong prec)
{
    arb_t c, rres;
    slong i;

	if(a == res) arb_init(rres);
	else arb_swap(rres,res);

	arb_init(c);
	arb_zero(rres);
    for(i=fmpq_poly_degree(pol); i>=0; i--)
	{
        arb_mul(rres, rres, a, prec);
		arb_set_fmpz(c, fmpq_poly_numref(pol)+i);
		arb_div_fmpz(c, c, fmpq_poly_denref(pol), prec);
		arb_add(rres, rres, c, prec);
	}

	arb_swap(rres, res);
	if(a == res) arb_clear(rres);
    arb_clear(c);
}

