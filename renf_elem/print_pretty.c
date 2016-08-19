/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "renf_elem.h"

void renf_elem_print_pretty(const renf_elem_t a, const renf_t nf, const char * var, slong prec)
{
    nf_elem_print_pretty(a->elem, nf->nf, var);
    printf(" in [");
    arb_printd(a->emb, prec);
    printf("]");
}
