/*
    Copyright (C) 2009 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#define FMPQ_POLY_INLINES_C

#define ulong ulongxx /* interferes with system includes */
#include <stdlib.h>
#include <stdio.h>
#undef ulong
#include <gmp.h>
#include <flint/flint.h>
#include <flint/ulong_extras.h>
#include <flint/fmpz.h>
#include <flint/fmpq.h>
#include <flint/fmpq_poly.h>

void fmpq_poly_get_coeff_fmpz(fmpz_t x, const fmpq_poly_t poly, slong n)
{
    if (n >= poly->length)  /* Coefficient is beyond the end of poly */
    {
        fmpz_zero(x);
        return;
    }
    
    fmpz_set(x, poly->coeffs + n);
}

