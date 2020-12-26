/*
    Copyright (C) 2016-2019 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifdef KIND

#define XPASTE2(X,Y) X ## Y
#define XPASTE3(X,Y,Z) X ## Y ## Z
#define PASTE2(X,Y) XPASTE2(X,Y)
#define PASTE3(X,Y,Z) XPASTE3(X,Y,Z)

#define RENF_ELEM_INT PASTE2(renf_elem_, KIND)
#define ARB_UNIQUE_INT PASTE3(arb_unique_, KIND, _fmpz)

#endif

static inline int 
ARB_UNIQUE_INT(fmpz_t res, arb_t x, arf_t cl, arf_t cr, slong prec)
{
    arb_get_interval_arf(cl, cr, x, prec);
    ARF_INT(cl, cl);
    ARF_INT(cr, cr);
    if (arf_equal(cl, cr))
    {
        arf_get_fmpz(res, cl, ARF_RND_NEAR);
        return 1;
    }
    return 0;
}

void
RENF_ELEM_INT(fmpz_t a, renf_elem_t b, renf_t nf)
{
    arf_t cl, cr;
    slong size, cond = WORD_MIN, prec;
    fmpz_t zsize;

    if (nf_elem_is_rational(b->elem, nf->nf))
    {
        if (nf->nf->flag & NF_LINEAR)
            FMPZ_DIV(a, LNF_ELEM_NUMREF(b->elem), LNF_ELEM_DENREF(b->elem));
        else if (nf->nf->flag & NF_QUADRATIC)
            FMPZ_DIV(a, QNF_ELEM_NUMREF(b->elem), QNF_ELEM_DENREF(b->elem));
        else
            FMPZ_DIV(a, NF_ELEM_NUMREF(b->elem), NF_ELEM_DENREF(b->elem));
        return;
    }

    /* ensure that the encolsure is not infinite */
    if (arf_is_inf(arb_midref(b->emb)) || mag_is_inf(arb_radref(b->emb)))
    {
        renf_elem_relative_condition_number_2exp(&cond, b, nf);
        prec = FLINT_MAX(nf->prec, arb_rel_accuracy_bits(nf->emb));
        renf_elem_set_evaluation(b, nf, prec + cond);

        while (arf_is_inf(arb_midref(b->emb)) || mag_is_inf(arb_radref(b->emb)))
        {
            prec *= 2;
            renf_refine_embedding(nf, prec);
            renf_elem_set_evaluation(b, nf, prec + cond);
        }
    }

    /* try default precision */
    arf_init(cl);
    arf_init(cr);
    if (ARB_UNIQUE_INT(a, b->emb, cl, cr, nf->prec))
    {
        arf_clear(cl);
        arf_clear(cr);
        return;
    }

    /* try higher precision and possibly refine the number field */
    fmpz_init(zsize);

    arf_abs_bound_lt_2exp_fmpz(zsize, cl);
    if (!fmpz_fits_si(zsize))
        abort();
    size = FLINT_MAX(fmpz_get_si(zsize) + 1, 1);

    arf_abs_bound_lt_2exp_fmpz(zsize, cr);
    if (!fmpz_fits_si(zsize))
        abort();
    size = FLINT_MAX(size, fmpz_get_si(zsize) + 1);

    fmpz_clear(zsize);

    prec = FLINT_MAX(nf->prec, arb_rel_accuracy_bits(nf->emb));

    if (cond == WORD_MIN)
        renf_elem_relative_condition_number_2exp(&cond, b, nf);

    do {
        renf_elem_set_evaluation(b, nf, prec + cond);
        if (ARB_UNIQUE_INT(a, b->emb, cl, cr, FLINT_MAX(size, prec)))
        {
            arf_clear(cl);
            arf_clear(cr);
            return;
        }

        prec *= 2;
        renf_refine_embedding(nf, prec);
    } while (1);
}
