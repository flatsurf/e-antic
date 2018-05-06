/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/poly_extra.h"
#include <ctype.h>
#include <stdio.h>

void check_valid(char * w, char * scoeff, slong pow)
{
    fmpq_t coeff, coeff2;
    slong pow2;
    int ans1, ans2;

    fmpq_init(coeff);
    fmpq_init(coeff2);

    ans1 = fmpq_set_str(coeff, scoeff, 10);
    ans2 = monomial_set_str(coeff2, &pow2, w);

    if (ans1 || ans2 || !fmpq_equal(coeff, coeff2) || pow != pow2)
    {
        printf("input: %s\n", w);
        printf("expected coeff %s and expected pow %d\n", scoeff, pow);
        printf(" (coeff = "); fmpq_print(coeff); printf(")\n");
        printf("ans1  : %d\n", ans1);
        printf("ans2  : %d\n", ans1);
        if (ans2 == 0)
        {
            printf("got coeff = "); fmpq_print(coeff2); printf("\n");
            printf("got pow   = %d\n", pow2);
        }
        abort();
    }

    fmpq_clear(coeff);
    fmpq_clear(coeff2);

    printf("---------------------------------------------------\n");
}

void check_invalid(char *w)
{
    fmpq_t coeff;
    slong pow;
    int ans;

    fmpq_init(coeff);
    ans = monomial_set_str(coeff, &pow, w);
    if (ans == 0)
    {
        printf("got ans = 0 for the invalid string %s\n", w);
        printf("coeff = "); fmpq_print(coeff); printf("\n");
        printf("pow   = %d\n", pow);
        abort();
    }
    fmpq_clear(coeff);
}

int main(void)
{
    check_valid("12/5", "12/5", 0);
    check_valid(" - 12/5 * a^3", "-12/5", 3);
    check_valid("-37/5 * a", "-37/5", 1);
    check_valid("a", "1", 1);
    check_valid("-a", "-1", 1);
    check_valid("a^7", "1", 7);
    check_valid("2/3a^5", "2/3", 5);
    check_valid("- 5/7 a^2", "-5/7", 2);
    check_valid("0a^7", "0", 7);
    check_valid("+1", "1", 0);

    check_invalid("-12*5");
    check_invalid("5+3");
    check_invalid("5^a");
    check_invalid("1^1");
    check_invalid("1*a^5a");
    check_invalid("a^b");
}
