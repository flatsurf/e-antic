/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of nf_emb

    nf_emb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "nf_emb_elem.h"

void nf_emb_elem_print_pretty(const nf_emb_elem_t a, const nf_emb_t nf, const char * var)
{
	nf_elem_print_pretty(a->elem, nf->nf, var);
	printf(" in [");
	arb_printd(a->emb, 10);
	printf("]");
}


