/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "fmpq_poly_extra.h"

void fmpq_poly_newton_step_arb(arb_t res, const fmpq_poly_t pol, const fmpq_poly_t der, arb_t a, slong prec)
{
	arf_t m;
	arb_t arbm,rres;

	if(a == res) arb_init(rres);
	else arb_swap(rres, res);

	arf_init(m);
	arb_init(arbm);

	fmpq_poly_evaluate_arf(m, pol, arb_midref(a), prec);
	fmpq_poly_evaluate_arb(rres, der, a, prec);

	arb_set_arf(arbm, m);
	arb_div(rres, arbm, rres, prec);
	arb_sub_arf(rres, rres, arb_midref(a), prec);
	arb_neg(rres, rres);

	arb_swap(rres, res);
	if(a == res) arb_clear(rres);

	arf_clear(m);
	arb_clear(arbm);
}
