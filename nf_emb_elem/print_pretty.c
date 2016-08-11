/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "nf_emb_elem.h"

void nf_emb_elem_print_pretty(const nf_emb_elem_t a, const nf_emb_t nf, const char * var, slong prec)
{
    nf_elem_print_pretty(a->elem, nf->nf, var);
    printf(" in [");
    if(nf->flag & NF_EMB_REAL)
        arb_printd(NF_ELEM_REMB_REF(a), prec);
    else
        acb_printd(NF_ELEM_CEMB_REF(a), prec);
    printf("]");
}


