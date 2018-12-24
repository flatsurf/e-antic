/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renf_elem.h>
#include <string.h>

char * renf_elem_get_str_pretty(renf_elem_t a, const char * var, renf_t nf, slong n, int flag)
{
    char *s, *t;

    s = NULL;
    t = flint_malloc(1 * sizeof(char));
    t[0] = '\0';

    if (flag & EANTIC_STR_ALG)
    {
        // polynomial expression
        char * s = nf_elem_get_str_pretty(a->elem, var, nf->nf);

        if ((flag & EANTIC_STR_D) || (flag & EANTIC_STR_ARB))
        {
            t = flint_realloc(t, strlen(t) + strlen(s) + 4);
            strcat(t, s);
            strcat(t, " ~ ");
        }
        else
        {
            t = flint_realloc(t, strlen(t) + strlen(s) + 1);
            strcat(t, s);
        }
        flint_free(s);
    }

    if (flag & EANTIC_STR_D)
    {
        // output of get_d
        s = flint_malloc(20 * sizeof(char));
        sprintf(s, "%lf", renf_elem_get_d(a, nf, ARF_RND_NEAR));
        t = flint_realloc(t, strlen(t) + strlen(s) + 1);
        strcat(t, s);
        flint_free(s);
    }

    if (flag & EANTIC_STR_ARB)
    {
        char * s = arb_get_str(a->emb, n, 0);
        t = flint_realloc(t, strlen(t) + strlen(s) + 1);
        strcat(t, s);
        flint_free(s);
    }

    return t;
}
