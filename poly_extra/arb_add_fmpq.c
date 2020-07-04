/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <arb.h>
#include <e-antic/poly_extra.h>

void arb_add_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_add(a, b, x, prec);
    arb_clear(x);
}

void arb_sub_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_sub(a, b, x, prec);
    arb_clear(x);
}

void arb_fmpq_sub(arb_t a, const fmpq_t c, const arb_t b, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_sub(a, x, b, prec);
    arb_clear(x);
}

void arb_mul_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_mul(a, b, x, prec);
    arb_clear(x);
}

void arb_div_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_div(a, b, x, prec);
    arb_clear(x);
}

void arb_fmpq_div(arb_t a, const fmpq_t c, const arb_t b, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_div(a, x, b, prec);
    arb_clear(x);
}
