/*
    Copyright (C) 2019 Vincent Delecroix
    Copyright (C) 2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <string.h>

#include "../e-antic/renf.h"

char * renf_get_str(const renf_t nf, const char * var, slong prec)
{
    char * p = fmpq_poly_get_str_pretty(nf->nf->pol, var);
    char * e = arb_get_str(nf->emb, prec, 0);
    char * res = (char *) flint_calloc(15 + strlen(p) + strlen(e) + 1, sizeof(char));
    res[0] = '\0';
    strcat(res, "NumberField(");
    strcat(res, p);
    strcat(res, ", ");
    strcat(res, e);
    strcat(res, ")");
    flint_free(p);
    flint_free(e);
    return res;
}
