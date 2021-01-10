/*
    Copyright (C) 2016 Vincent Delecroix
                  2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "../e-antic/renf_elem.h"

int renf_elem_cmp(renf_elem_t a, renf_elem_t b, renf_t nf)
{
    renf_elem_t diff;
    slong prec, conda, condb;
    int s;

    /* equality */
    if (nf_elem_equal(a->elem, b->elem, nf->nf)) return 0;

    /* compare enclosures */
    if (arb_lt(a->emb, b->emb))
        return -1;
    if (arb_gt(a->emb, b->emb))
        return 1;

    /* rationals */
    if (nf_elem_is_rational(a->elem, nf->nf) &&
        nf_elem_is_rational(b->elem, nf->nf))
    {
        if (nf_elem_is_zero(a->elem, nf->nf))
            return -renf_elem_sgn(b, nf);

        if (nf_elem_is_zero(b->elem, nf->nf))
            return renf_elem_sgn(a, nf);

        if (nf->nf->flag & NF_LINEAR)
            return _fmpq_cmp(
                        LNF_ELEM_NUMREF(a->elem),
                        LNF_ELEM_DENREF(a->elem),
                        LNF_ELEM_NUMREF(b->elem),
                        LNF_ELEM_DENREF(b->elem));
        else if (nf->nf->flag & NF_QUADRATIC)
            return _fmpq_cmp(
                        QNF_ELEM_NUMREF(a->elem),
                        QNF_ELEM_DENREF(a->elem),
                        QNF_ELEM_NUMREF(b->elem),
                        QNF_ELEM_DENREF(b->elem));
        else
            return _fmpq_cmp(
                        NF_ELEM_NUMREF(a->elem),
                        NF_ELEM_DENREF(a->elem),
                        NF_ELEM_NUMREF(b->elem),
                        NF_ELEM_DENREF(b->elem));
    }

    /* try better enclosures */
    prec = FLINT_MAX(nf->prec, arb_rel_accuracy_bits(nf->emb));

    renf_elem_relative_condition_number_2exp(&conda, a, nf);
    renf_elem_set_evaluation(a, nf, prec + conda);

    renf_elem_relative_condition_number_2exp(&condb, b, nf);
    renf_elem_set_evaluation(b, nf, prec + condb);

    if (arb_lt(a->emb, b->emb))
        return -1;
    if (arb_gt(a->emb, b->emb))
        return 1;

    /* sign of the difference */
    renf_elem_init(diff, nf);
    renf_elem_sub(diff, a, b, nf);
    s = renf_elem_sgn(diff, nf);
    renf_elem_clear(diff, nf);
    return s;
}

