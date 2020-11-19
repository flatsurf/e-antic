/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <arb.h>

#include "../e-antic/renf_elem.h"

void renf_elem_fdiv(fmpz_t a, renf_elem_t b, renf_elem_t c, renf_t nf)
{
    if (!renf_elem_is_rational(c, nf))
    {
        slong bcond = WORD_MIN;
        slong ccond = WORD_MIN;
        slong prec;

        prec = FLINT_MAX(nf->prec, arb_rel_accuracy_bits(nf->emb));
        renf_elem_relative_condition_number_2exp(&bcond, b, nf);
        renf_elem_relative_condition_number_2exp(&ccond, c, nf);

        renf_elem_set_evaluation(b, nf, prec + bcond);

        /* ensure that the enclosure of the divident is not infinite */
        while (!arb_is_finite(b->emb))
        {
            prec *= 2;
            renf_refine_embedding(nf, prec);
            renf_elem_set_evaluation(b, nf, prec + bcond);
        }

        renf_elem_set_evaluation(c, nf, prec + ccond);

        /* ensure that the enclosure of the divisor does not contain zero */
        while (!arb_is_nonzero(c->emb))
        {
            prec *= 2;
            renf_refine_embedding(nf, prec);
            renf_elem_set_evaluation(c, nf, prec + ccond);
        }

        arb_t quotient;
        arb_init(quotient);

        /* Refine until the approximation of the quotient contains at most one integer. */
        arb_div(quotient, b->emb, c->emb, prec);
        while (mag_cmp_2exp_si(arb_radref(quotient), -1) >= 0)
        {
            prec *= 2;
            renf_refine_embedding(nf, prec);

            renf_elem_set_evaluation(b, nf, prec + bcond);
            renf_elem_set_evaluation(c, nf, prec + ccond);
            
            arb_div(quotient, b->emb, c->emb, prec);
        }

        if (arb_contains_int(quotient))
        {
            arb_get_unique_fmpz(a, quotient);

            renf_elem_t ac;
            renf_elem_init(ac, nf);
            renf_elem_mul_fmpz(ac, c, a, nf);

            if (renf_elem_cmp(ac, b, nf) == renf_elem_sgn(c, nf))
                fmpz_add_si(a, a, -1);

            renf_elem_clear(ac, nf);
        }
        else
        {
            arf_get_fmpz(a, arb_midref(quotient), ARF_RND_FLOOR);
        }

        arb_clear(quotient);
    }
    else
    {
        /* For a rational divisior it's faster to perform exact division and
         * then take the floor of the result. */
        renf_elem_t quotient;
        renf_elem_init(quotient, nf);
        renf_elem_div(quotient, b, c, nf);
        renf_elem_floor(a, quotient, nf);
        renf_elem_clear(quotient, nf);
    }
}
