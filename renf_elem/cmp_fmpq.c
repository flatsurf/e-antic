/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renf_elem.h>

/* TODO: we might want to move this to ANTIC */
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
    arb_t bi, difference;

    if (_nf_equal_fmpq(a->elem, b, nf->nf)) return 0;

    arb_init(bi);
    arb_init(difference);
    arb_set_fmpq(bi, b, nf->prec); 
    arb_sub(difference, a->emb, bi, nf->prec);

    if (arb_contains_zero(difference))
    {
        slong prec = nf->prec;
        do
        {
            prec *= 2;
            renf_refine_embedding(nf, prec);
            arb_set_fmpq(bi, b, prec); 
            renf_elem_set_evaluation(a, nf, prec);
            arb_sub(difference, a->emb, bi, prec);
        } while (arb_contains_zero(difference));
    }

    s = arf_sgn(arb_midref(difference));
    arb_clear(difference);
    arb_clear(bi);
    return s;
}
