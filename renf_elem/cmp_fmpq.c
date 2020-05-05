/*
    Copyright (C) 2017 Vincent Delecroix
                  2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renf_elem.h>

/* TODO: move it to ANTIC? */
int _nf_equal_fmpq(const nf_elem_t a, const fmpq_t b, const nf_t nf)
{
    if (nf->flag & NF_LINEAR)
    {
        return (fmpz_equal(LNF_ELEM_NUMREF(a), fmpq_numref(b)) &&
                fmpz_equal(LNF_ELEM_DENREF(a), fmpq_denref(b)));
    }
    else if (nf->flag & NF_QUADRATIC)
    {
        const fmpz * const num = QNF_ELEM_NUMREF(a);
        const fmpz * const den = QNF_ELEM_DENREF(a);
        return (fmpz_is_zero(num + 1) &&
                fmpz_equal(num, fmpq_numref(b)) &&
                fmpz_equal(den, fmpq_denref(b)));
    }
    else
    {
        if (NF_ELEM(a)->length > 1) return 0;
        else if (NF_ELEM(a)->length == 0) return fmpq_is_zero(b);
        return (fmpz_equal(NF_ELEM_NUMREF(a), fmpq_numref(b)) &&
                fmpz_equal(NF_ELEM_DENREF(a), fmpq_denref(b)));
    }

    return -1;
}

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
