/*
    Copyright (C) 2016, Vincent Delecroix

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

int main()
{
    fmpz_poly_t p;

    fmpz_poly_init(p);

    fmpz_poly_set_coeff_si(p, 0, 1);
    fmpz_poly_set_coeff_si(p, 1, -1);
    fmpz_poly_set_coeff_si(p, 2, 1);
    fmpz_poly_set_coeff_si(p, 3, -1);
    fmpz_poly_set_coeff_si(p, 4, 1);

    fmpz_poly_print_pretty(p, "x"); printf("\n");

    printf("+1: ");
    _fmpz_poly_scale_2exp(p->coeffs, p->length, 1);
    fmpz_poly_print_pretty(p, "x"); printf("\n");

    printf("-1: ");
    _fmpz_poly_scale_2exp(p->coeffs, p->length, -1);
    fmpz_poly_print_pretty(p, "x"); printf("\n");

    printf("-1: ");
    _fmpz_poly_scale_2exp(p->coeffs, p->length, -1);
    fmpz_poly_print_pretty(p, "x"); printf("\n");

    printf("+1: ");
    _fmpz_poly_scale_2exp(p->coeffs, p->length, 1);
    fmpz_poly_print_pretty(p, "x"); printf("\n");

    return 0;
}
