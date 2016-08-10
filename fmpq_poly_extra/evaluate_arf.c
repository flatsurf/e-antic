/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of nf_emb

    nf_emb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "fmpq_poly_extra.h"

void fmpq_poly_evaluate_arf(arf_t res, const fmpq_poly_t pol, const arf_t a, slong prec)
{
    arf_t c, rres;
    slong i;

	if(a == res) arf_init(rres);
	else arf_swap(rres, res);

	arf_init(c);
	arf_zero(rres);
    for(i=fmpq_poly_degree(pol); i>=0; i--)
	{
        arf_mul(rres, rres, a, prec, ARF_RND_NEAR);
		arf_set_fmpz(c, fmpq_poly_numref(pol)+i);
		arf_div_fmpz(c, c, fmpq_poly_denref(pol), prec, ARF_RND_NEAR);
		arf_add(rres, rres, c, prec, ARF_RND_NEAR);
	}
	
	arf_swap(rres,res);
	if(a==res) arf_clear(rres);
    arf_clear(c);
}


