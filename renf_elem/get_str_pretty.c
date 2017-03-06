/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "renf_elem.h"
#include <string.h>

char * renf_elem_get_str_pretty(const renf_elem_t a, const char * var, const renf_t nf, slong n)
{
    char * x1 = nf_elem_get_str_pretty(a->elem, var, nf->nf);
    char * x2 = arb_get_str(a->emb, n, 0);
    char * res = flint_malloc((strlen(x1) + strlen(x2) + 10) * sizeof(char));

    strcpy(res, x1);
    strcat(res, " in ");
    strcat(res, x2);

    flint_free(x1);
    flint_free(x2);

    return res;
}
