/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "nf_emb.h"

int main()
{
	fmpq_poly_t p;
	arb_t emb;
	nf_emb_t nf;

	fmpq_poly_init(p);
	arb_init(emb);

	/* x^2 - 2 */
	fmpq_poly_set_coeff_si(p, 0, -2);
	fmpq_poly_set_coeff_si(p, 2, 1);
	arb_set_d(emb, 1.414213562373095);
	arb_add_error_2exp_si(emb, -20);
	nf_emb_init(nf, p, emb);
	nf_emb_clear(nf);

	/* x^4 - 100*x^3 + 50*x^2 - 20*x + 1 */
	fmpq_poly_zero(p);
	fmpq_poly_set_coeff_si(p, 4, 1);
	fmpq_poly_set_coeff_si(p, 3, -100);
	fmpq_poly_set_coeff_si(p, 2, 50);
	fmpq_poly_set_coeff_si(p, 1, -20);
	fmpq_poly_set_coeff_si(p, 0, 1);
	arb_set_d(emb, 99.4995040886108);
	arb_add_error_2exp_si(emb, -20);
	nf_emb_init(nf, p, emb);
	nf_emb_clear(nf);

	/* p = x^30 - 3000*x - 1 */
	fmpq_poly_zero(p);
	fmpq_poly_set_coeff_si(p, 30, 1);
	fmpq_poly_set_coeff_si(p, 1, -3000);
	fmpq_poly_set_coeff_si(p, 0, -1);
	arb_set_d(emb, 1.317966950615985);
	arb_add_error_2exp_si(emb, -20);
	nf_emb_init(nf, p, emb);
	nf_emb_clear(nf);

	return 0;
}
