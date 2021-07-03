/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <arb.h>
#include <string.h>

#include "../e-antic/renf_elem.h"

char * renf_elem_get_str_pretty(renf_elem_t a, const char * var, renf_t nf, slong n, int flag)
{
    char *t;

    t = flint_malloc(1 * sizeof(char));
    t[0] = '\0';

    if (flag & EANTIC_STR_ALG)
    {
        // polynomial expression
        char * s = nf_elem_get_str_pretty(a->elem, var, nf->nf);

        if ((flag & EANTIC_STR_D) || (flag & EANTIC_STR_ARB))
        {
            t = flint_realloc(t, strlen(s) + 4);
            strcat(t, s);
            strcat(t, " ~ ");
        }
        else
        {
            t = flint_realloc(t, strlen(s) + 1);
            strcat(t, s);
        }
        flint_free(s);
    }

    if (flag & EANTIC_STR_D)
    {
        // Print the approximate double value of a.
        char* s = NULL;

        double d = renf_elem_get_d(a, nf, ARF_RND_NEAR);

        if (isinf(d))
        {
            if (d < 0)
            {
                s = flint_malloc(strlen("-inf") + 1);
                strcpy(s, "-inf");
            }
            else
            {
                s = flint_malloc(strlen("inf") + 1);
                strcpy(s, "inf");
            }
        }
        else
        {
            // Since we do not like printf()'s double printing, (the output of %f
            // is too large for big numbers, %e is hard to read for small integers,
            // %g does not show enough digits in many cases,) we use Arb's double
            // printing instead.
            arb_t x;
            arb_init(x);

            arb_set_d(x, d);

            s = arb_get_str(x, 8, ARB_STR_NO_RADIUS);

            arb_clear(x);
        }

        t = flint_realloc(t, strlen(t) + strlen(s) + 1);
        strcat(t, s);

        flint_free(s);
    }

    else if (flag & EANTIC_STR_ARB)
    {
        char * s = arb_get_str(a->emb, n, 0);
        t = flint_realloc(t, strlen(t) + strlen(s) + 1);
        strcat(t, s);
        flint_free(s);
    }

    return t;
}
