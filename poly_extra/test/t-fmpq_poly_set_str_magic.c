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

void check_valid(char * s, char * ans)
{
    fmpq_poly_t p, pans;
    int err1, err2;

    fmpq_poly_init(p);
    fmpq_poly_init(pans);

    err1 = fmpq_poly_set_str_magic(p, s);
    err2 = fmpq_poly_set_str(pans, ans);

    if (err1 || err2)
    {
        printf("Got err1=%d with input s='%s'\n", err1, s);
        printf("Got err2=%d with input ans='%s'\n", err2, ans);
        abort();
    }
    if (!fmpq_poly_equal(p, pans))
    {
        printf("Expecting pans="); fmpq_poly_print(pans); printf("\n");
        printf("Got p="); fmpq_poly_print(p); printf("\n");
        abort();
    }

    printf("s = '%s'\n", s);
    printf("p = "); fmpq_poly_print(p); printf("\n");
    printf("-----------------------------------\n");

    fmpq_poly_clear(p);
    fmpq_poly_clear(pans);
}


int main(void)
{
    check_valid("1", "1  1");
    check_valid("a", "2  0 1");
    check_valid("3/5*a^2", "3  0 0 3/5");
    check_valid("1+a+a^2", "3  1 1 1");
    check_valid("3/5a^2 + -1/2", "3  -1/2 0 3/5");
    check_valid("-a^3", "4  0 0 0 -1");
    check_valid("1 - a", "2  1 -1");
    check_valid("2  3/7 -2", "2  3/7 -2");
    check_valid("0", "0");
    check_valid("1", "1  1");
    check_valid("3/5", "1  3/5");
    check_valid("-1", "1  -1");
    check_valid("3/5*a - 7/3", "2  -7/3 3/5");
    check_valid("1/3 + 3/5 * a^3 - 7/3", "4  -2 0 0 3/5");

}
