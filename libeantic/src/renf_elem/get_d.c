/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <float.h>
#include <assert.h>

#include "../e-antic/renf_elem.h"

#if FLT_RADIX != 2
#error "not implemented: doubles that are not binary internally"
#endif

static int renf_elem_overflows_d(renf_elem_t a, renf_t nf)
{
    if (renf_elem_sgn(a, nf) >= 0)
    {
        if (renf_elem_cmp_ui(a, UINT_MAX, nf) <= 0)
            return 0;

        fmpz_t dbl_max;
        fmpz_init(dbl_max);
        fmpz_set_d(dbl_max, DBL_MAX);
        int cmp = renf_elem_cmp_fmpz(a, dbl_max, nf);
        fmpz_clear(dbl_max);
        return cmp == 1 ? 1 : 0;
    } else {
        renf_elem_neg(a, a, nf);
        int overflow = renf_elem_overflows_d(a, nf);
        renf_elem_neg(a, a, nf);

        return -overflow;
    }
}

double renf_elem_get_d(renf_elem_t a, renf_t nf, arf_rnd_t rnd)
{
    slong prec = nf->prec;

    if (renf_elem_is_zero(a, nf))
        return 0.;

    if (renf_elem_is_rational(a, nf))
    {
        /* Delegate conversion of rational numbers to arf. */
        fmpq_t b;
        arf_t c;

        fmpq_init(b);
        arf_init(c);

        nf_elem_get_coeff_fmpq(b, a->elem, 0, nf->nf);
        arf_set_fmpq(c, b, DBL_MANT_DIG, rnd);

        double d = arf_get_d(c, rnd);

        arf_clear(c);
        fmpq_clear(b);

        return d;
    }

    {
        /* Reduce rnd so we only need to handle the cases FLOOR, NEAR, and CEIL. */
        if (rnd == ARF_RND_UP)
          rnd = renf_elem_sgn(a, nf) > 0 ? ARF_RND_CEIL : ARF_RND_FLOOR;
        if (rnd == ARF_RND_DOWN)
          rnd = renf_elem_sgn(a, nf) > 0 ? ARF_RND_FLOOR : ARF_RND_CEIL;
    }

    {
        /* We treat any number outside of the finite double range as
         * ±∞. One could argue that the floor of a very big number
         * should always be the maximum double but that typically
         * leads to confusing behaviour. Also, floor of DBL_MAX +
         * epsilon should probably give DBL_MAX and not +∞ but that's
         * quite a bit harder to implement. */

        int overflow = renf_elem_overflows_d(a, nf);

        if (overflow == 1)
            return 1./0.;
        if (overflow == -1)
            return -1./0.;
    }

    {
        /* Since we know that this number cannot be exactly
         * represented by a double (since it's not rational,) we can
         * now successively shrink the ball around its approximate
         * value until we can decide which double we should round it
         * to.
         * We will certainly need 53 bits to see the full mantissa.
         * Since we ask for an additional bit, the ball around our
         * approximation contains at most one double. Since we ask for
         * yet another bit, we know that any double contained in that
         * ball is closer to every point of the ball than any other
         * double. */
        if (arb_rel_accuracy_bits(a->emb) < DBL_MANT_DIG + 2)
        {
            if(2 * arf_bits(arb_midref(a->emb)) < prec)
                renf_elem_set_evaluation(a, nf, prec);

            while (arb_rel_accuracy_bits(a->emb) < DBL_MANT_DIG + 2)
            {
                prec *= 2;
                renf_refine_embedding(nf, prec);
                renf_elem_set_evaluation(a, nf, prec);
            }
        }

        prec = FLINT_MAX(nf->prec, arb_rel_accuracy_bits(a->emb));
    }

    {
        arf_t lbound, ubound;

        arf_init(lbound);
        arf_init(ubound);

        double d;

        while (1)
        {
            arb_get_lbound_arf(lbound, a->emb, prec);
            arb_get_ubound_arf(ubound, a->emb, prec);

            d = arf_get_d(lbound, rnd);
            if (arf_get_d(ubound, rnd) == d)
                break;

            prec *= 2;
            renf_refine_embedding(nf, prec);
            renf_elem_set_evaluation(a, nf, prec);
        }

        arf_clear(lbound);
        arf_clear(ubound);

        return d;
    }
}
