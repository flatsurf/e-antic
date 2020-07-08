/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifdef NUMBER

#define PASTE2(X,Y) X ## Y
#define PASTE(X,Y) PASTE2(X,Y)

#define NUM_T PASTE(NUMBER, _t)
#define EANTIC_FMPZ_POLY_EVALUATE PASTE(_fmpz_poly_evaluate_, NUMBER)
#define FMPZ_POLY_EVALUATE PASTE(fmpz_poly_evaluate_, NUMBER)
#define FMPQ_POLY_EVALUATE PASTE(fmpq_poly_evaluate_, NUMBER)

#ifndef INIT
#define INIT(x) PASTE(NUMBER, _init)(x)
#endif

#ifndef ZERO
#define ZERO(x) PASTE(NUMBER, _zero)(x)
#endif

#ifndef MUL
#define MUL(z,x,y,prec) PASTE(NUMBER, _mul)(z,x,y,prec)
#endif

#ifndef ADD_FMPZ
#define ADD_FMPZ(z,x,y,prec) PASTE(NUMBER, _add_fmpz)(z,x,y,prec)
#endif

#ifndef DIV_FMPZ
#define DIV_FMPZ(z,x,y,prec) PASTE(NUMBER, _div_fmpz)(z,x,y,prec)
#endif

#ifndef SWAP
#define SWAP(x,y) PASTE(NUMBER, _swap)(x,y)
#endif

#ifndef CLEAR
#define CLEAR(x) PASTE(NUMBER, _clear)(x)
#endif

void EANTIC_FMPZ_POLY_EVALUATE(NUM_T res, const fmpz * pol, slong len, const NUM_T a, slong prec)
{
    slong i;

    ZERO(res);
    for (i = len - 1; i >= 0; i--)
    {
        MUL(res, res, a, prec);
        ADD_FMPZ(res, res, pol+i, prec);
    }
}

void FMPZ_POLY_EVALUATE(NUM_T res, const fmpz_poly_t pol, const NUM_T a, slong prec)
{
    NUM_T rres;

    if (a == res) INIT(rres);
    else SWAP(rres, res);

    EANTIC_FMPZ_POLY_EVALUATE(rres, pol->coeffs, fmpz_poly_length(pol), a, prec);

    SWAP(rres, res);
    if (a == res) CLEAR(rres);
}

void FMPQ_POLY_EVALUATE(NUM_T res, const fmpq_poly_t pol, const NUM_T a, slong prec)
{
    NUM_T rres;

    if (a == res) INIT(rres);
    else SWAP(rres, res);

    EANTIC_FMPZ_POLY_EVALUATE(rres, fmpq_poly_numref(pol), fmpq_poly_length(pol), a, prec);
    DIV_FMPZ(rres, rres, fmpq_poly_denref(pol), prec);

    SWAP(rres, res);
    if (a == res) CLEAR(rres);
}

#undef PASTE2
#undef PASTE
#undef NUM_T
#undef EANTIC_FMPZ_POLY_EVALUATE
#undef FMPZ_POLY_EVALUATE
#undef INIT
#undef ZERO
#undef MUL
#undef ADD_FMPZ
#undef DIV_FMPZ
#undef SWAP
#undef CLEAR

#endif
