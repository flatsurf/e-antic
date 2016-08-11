/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

void _fmpz_poly_newton_step_acb(acb_t res, const fmpz * pol, const fmpz * der, slong len, acb_t a, slong prec)
{
	fprintf(stderr, "NOT IMPLEMENTED");
	exit(EXIT_FAILURE);
}

void fmpz_poly_newton_step_acb(acb_t res, const fmpz_poly_t pol, const fmpz_poly_t der, acb_t a, slong prec)

{
    acb_t rres;

    if(a == res) acb_init(rres);
    else acb_swap(rres, res);

    _fmpz_poly_newton_step_acb(res, pol->coeffs, der->coeffs, fmpz_poly_length(pol), a, prec);

    acb_swap(rres, res);
    if(a == res) acb_clear(rres);

}
