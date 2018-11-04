/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/poly_extra.h"
#include <stdio.h>
#include <string.h>

void check_valid(char * s, char * var, char * ans)
{
    fmpq_poly_t p1, p2;
    int err1, err2;

    fmpq_poly_init(p1);
    fmpq_poly_init(p2);

    err1 = fmpq_poly_set_str_pretty(p1, s, var);
    err2 = fmpq_poly_set_str(p2, ans);

    if (err1 || err2)
    {
        printf("Got err1=%d with input s='%s'\n", err1, s);
        printf("Got err2=%d with input ans='%s'\n", err2, ans);
        if (err1 == 0)
        {
            printf("Got p1="); fmpq_poly_print(p1); printf("\n");
        }
        if (err2 == 0)
        {
            printf("Got p2="); fmpq_poly_print(p2); printf("\n");
        }

        flint_abort();
    }
    if (!fmpq_poly_equal(p1, p2))
    {
        printf("Got p1="); fmpq_poly_print(p1); printf("\n");
        printf("Got p2="); fmpq_poly_print(p2); printf("\n");
        flint_abort();
    }

    fmpq_poly_clear(p1);
    fmpq_poly_clear(p2);
}

void check_invalid(char *w, char * var)
{
    fmpq_poly_t p;
    int ans;

    fmpq_poly_init(p);
    ans = fmpq_poly_set_str_pretty(p, w, var);
    if (ans == 0)
    {
        printf("got ans = 0 for the invalid string %s\n", w);
        printf("coeff = "); fmpq_poly_print(p); printf("\n");
        abort();
    }
    fmpq_poly_clear(p);
}


int main(void)
{
    /* single coefficient */
    check_valid("1", "x", "1  1");
    check_valid("0", "a", "0");
    check_valid("3/5", "a", "1  3/5");
    check_valid("-1", "a", "1  -1");
    check_valid("-15/2", "x", "1  -15/2");
    check_valid(" - 15/2", "x", "1  -15/2");
    check_valid(" - +    -15/2  ", "x", "1  15/2");

    /* variable names */
    check_valid("a", "a", "2  0 1");
    check_valid("a2", "a2", "2  0 1");
    check_valid("a_3", "a_3", "2  0 1");
    check_valid("aBc", "aBc", "2  0 1");

    /* monomials */
    check_valid("a2", "a", "3  0 0 1");
    check_valid("a22", "a2", "3  0 0 1");
    check_valid("3 a2", "a", "3  0 0 3");
    check_valid("3/5*a^2", "a", "3  0 0 3/5");
    check_valid("3/5a2", "a", "3  0 0 3/5");

    /* polynomials */
    check_valid("1+a +  a  ^   2    ", "a", "3  1 1 1");
    check_valid("3/5a^2 + -1/2", "a", "3  -1/2 0 3/5");
    check_valid("-a^3", "a", "4  0 0 0 -1");
    check_valid("1 - a", "a", "2  1 -1");
    check_valid("a^2 + 2/5 a", "a", "3  0 2/5 1");
    check_valid("3/5*a - 7/3", "a", "2  -7/3 3/5");
    check_valid("1/3 + 3/5 * a^3 - 7/3", "a", "4  -2 0 0 3/5");

    /* invalid */
    check_invalid("3x3x", "x");
    check_invalid("x2x", "x");
    check_invalid("a", "aa");
    check_invalid("a*a", "a");
    check_invalid("3^23", "a");
    check_invalid("3a", "3a");
    check_invalid(" a", " a");
    check_invalid("1 + 3a", "x");
    check_invalid("1 + 3aa", "a");
}
