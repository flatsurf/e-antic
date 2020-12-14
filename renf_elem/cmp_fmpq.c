/*
    Copyright (C) 2017 Vincent Delecroix
                  2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <assert.h>

#include "../e-antic/renf_elem.h"

int renf_elem_cmp_fmpq(renf_elem_t a, const fmpq_t b, renf_t nf)
{
    int s;
    slong prec, cond;
    arb_t diffball;
    renf_elem_t diffnf;

    if (fmpq_is_zero(b))
        return renf_elem_sgn(a, nf);

    if (nf_elem_is_zero(a->elem, nf->nf))
        return -fmpq_sgn(b);

    if (nf_elem_is_rational(a->elem, nf->nf))
    {
        if (nf->nf->flag & NF_LINEAR)
            return _fmpq_cmp(LNF_ELEM_NUMREF(a->elem),
                             LNF_ELEM_DENREF(a->elem),
                             fmpq_numref(b),
                             fmpq_denref(b));
        else if (nf->nf->flag & NF_QUADRATIC)
            return _fmpq_cmp(QNF_ELEM_NUMREF(a->elem),
                             QNF_ELEM_DENREF(a->elem),
                             fmpq_numref(b),
                             fmpq_denref(b));
        else
            return _fmpq_cmp(NF_ELEM_NUMREF(a->elem),
                             NF_ELEM_DENREF(a->elem),
                             fmpq_numref(b),
                             fmpq_denref(b));
    }

    arb_init(diffball);
    arb_set_fmpq(diffball, b, nf->prec);
    arb_sub(diffball, a->emb, diffball, nf->prec);

    if (!arb_contains_zero(diffball))
    {
        s = arf_sgn(arb_midref(diffball));
        arb_clear(diffball);
        return s;
    }

    renf_elem_relative_condition_number_2exp(&cond, a, nf);
    prec = FLINT_MAX(nf->prec, arb_rel_accuracy_bits(nf->emb));

    assert(prec != -cond && "prec + cond == 0 which does not make sense. In mid 2020 we found this to be the case sometimes. Unfortunately, we never managed to debug this issue. The only reproducer needed several days of computation to get here. So if this happens to you, please let us know.");

    renf_elem_set_evaluation(a, nf, prec + cond);

    arb_set_fmpq(diffball, b, prec);
    arb_sub(diffball, a->emb, diffball, prec);

    if (!arb_contains_zero(diffball))
    {
        s = arf_sgn(arb_midref(diffball));
        arb_clear(diffball);
        return s;
    }

    arb_clear(diffball);
    renf_elem_init(diffnf, nf);
    renf_elem_set(diffnf, a, nf);
    renf_elem_sub_fmpq(diffnf, diffnf, b, nf);
    s = renf_elem_sgn(diffnf, nf);
    renf_elem_clear(diffnf, nf);
    return s;
}
