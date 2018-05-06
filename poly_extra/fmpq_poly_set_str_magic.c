/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "flint/fmpq_poly.h"
#include "e-antic/poly_extra.h"
#include <ctype.h>
#include <string.h>

int fmpq_poly_set_str_magic(fmpq_poly_t p, const char * s)
/* parse several kind of strings:     */
/* integers    :  23 or -2               */
/* rationals   :  123/45 or -2/5         */
/* coefficients: 2  -3 5/2        */
/* polynomial  : 2*a^5 + 5/3*a - 2 */
{
    int pretty = 0;
    int has_space = 0;
    int has_div = 0;
    int has_minus = 0;
    char var = '\0';

    /* run a first time through s to see what kind of data it is */
    {
        const char * t;
        for (t = s; *t != '\0'; t++)
        {
            if (isdigit(*t))
                continue;

            else if (*t == ' ')
                has_space = 1;

            else if (*t == '/')
                has_div = 1;

            else if (*t == '-')
                has_minus = 1;

            else if (*t == '^' || *t == '+' || *t == '*')
               pretty = 1;

            else if (var != '\0' && var != *t)
                return -1;

            else if (isalpha(*t))
            {
                pretty = 1;
                var = *t;
            }
            else
                return -1;
        }
    }

    if (!pretty)
    {
        /* scalar or fmpq polynomial */
        if (has_space)
            return fmpq_poly_set_str(p, s);
        else
        {
            int ans;
            fmpq_t r;

            fmpq_init(r);
            ans = fmpq_set_str(r, s, 10);
            if (ans)
            {
                fmpq_clear(r);
                return -1;
            }
            fmpq_poly_set_fmpq(p, r);
            fmpq_clear(r);

            return 0;
        }
    }

    /* pretty polynomial */
    char * z;

    /* Find maximal gap between spaces and allocate w */
    {
        const char * k = s;
        slong max;
        for (max = 0; *k != '\0';)
        {
            slong cur;
            for (k++, cur = 1; *k != '+' && *k != '-' && *k != '\0'; k++, cur++) ;
            if (max < cur)
                max = cur;
        }

        z = (char *) flint_malloc((max + 1) * sizeof(char));
    }

    const char *start;
    fmpq_t coeff;
    slong pow;
    fmpq_poly_t mon;

    fmpq_poly_init(mon);
    fmpq_poly_zero(p);
    start = s;

    while (*start != '\0')
    {
        int ans;
        const char *end = start;

        /* jump over sign */
        while (*end == '+' || *end == ' ' || *end == '-') end++;

        /* jump over non sign */
        while (*end != '\0' && *end != '+' && *end != '-') end++;

        strncpy(z, start, end - start);
        z[end - start] = '\0';

        ans = monomial_set_str(coeff, &pow, z);
        if (ans)
        {
            fmpq_poly_clear(mon);
            flint_free(z);
            return -1;
        }
        fmpq_poly_zero(mon);
        fmpq_poly_set_coeff_fmpq(mon, pow, coeff);
        fmpq_poly_add(p, p, mon);

        start = end;
    }

    fmpq_poly_clear(mon);
    flint_free(z);

    return 0;
}
