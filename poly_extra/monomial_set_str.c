/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/poly_extra.h"
#include "flint/fmpq.h"
#include <errno.h>
#include <ctype.h>
#include <string.h>

int monomial_set_str(fmpq_t coeff, slong * pow, char * w)
{
    char * x = w;
    char * y;
    char * z;
    int sign = 1;
    int ans;

    /* ignore spaces */
    while (*x == ' ') x++;

    if (*x == '\0')
        return -1;

    /* 1. sign */
    while (1)
    {
        if (*x == '\0')
            return -1;
        else if (*x == '-')
            sign *= -1;
        else if (*x != ' ' && *x != '+')
            break;
        x++;
    }

    /* 2. optional coefficient */
    if (isdigit(*x))
    {
        y = x + 1;
        while (isdigit(*y) || (*y == '/')) y++;

        z = (char *) flint_malloc((y - x + 1) * sizeof(char));
        strncpy(z, x, y - x);
        z[y - x] = '\0';
        ans = fmpq_set_str(coeff, z, 10);
        flint_free(z);
        if (ans)
            return ans;
        x = y;
    }
    else
        fmpq_one(coeff);

    if (sign == -1)
        fmpq_neg(coeff, coeff);

    while (*x == ' ') x++;

    if (*x == '\0')
    {
        *pow = 0;
        return 0;
    }

    /* 3. variable */

    if (*x == '*') x++;

    while (*x == ' ') x++;
    if (*x == '\0' || !isalpha(*x))
        return -1;
    x++;
    while (*x == ' ') x++;
    if (*x == '\0')
    {
        *pow = 1;
        return 0;
    }


    /* 4. power */
    if (*x != '^')
        return -1;
    x++;
    if (*x == '\0' || !isdigit(*x))
        return -1;

    errno = 0;
    *pow = strtol(x, &y, 10);
    if (errno)
        return -1;

    while (*y == ' ') y++;

    if (*y != '\0')
        return -1;

    return 0;
}
