/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "../e-antic/fmpz_poly_extra.h"
#include "../e-antic/fmpq_poly_extra.h"

static __inline__ int
arb_sgn2(arb_t a)
{
    if (arb_is_positive(a)) return 1;
    else if (arb_is_negative(a)) return -1;
    return 0;
}

int fmpq_poly_check_unique_real_root(const fmpq_poly_t pol, const arb_t a, slong prec)
{
    if (pol->length < 2)
        return 0;
    else if (pol->length == 2)
    {
        /* linear polynomial */
        fmpq_t root;
        int ans;

        fmpq_init(root);
        fmpq_set_fmpz_frac(root, fmpq_poly_numref(pol), fmpq_poly_numref(pol) + 1);
        fmpq_neg(root, root);
        ans = arb_contains_fmpq(a, root);
        fmpq_clear(root);
        return ans;
    }
    else
    {
        arb_t b, c;
        arf_t l, r;

        fmpz * der;
        int lsign, rsign;
        fmpz_poly_t pol2;
        slong n;

        /* 1 - cheap test:                    */
        /*   - sign(left) * sign(right) = -1  */
        /*   - no zero of the derivative      */
        arb_init(b);
        arb_init(c);
        arf_init(l);
        arf_init(r);
        arb_get_interval_arf(l, r, a, prec);
        arb_set_arf(b, l);
        _fmpz_poly_evaluate_arb(c, pol->coeffs, pol->length, b, 2*prec);
        lsign = arb_sgn2(c);

        arb_set_arf(b, r);
        _fmpz_poly_evaluate_arb(c, pol->coeffs, pol->length, b, 2*prec);
        rsign = arb_sgn2(c);

        arb_clear(c);
        if (lsign * rsign == -1)
        {
            der = _fmpz_vec_init(pol->length - 1);
            _fmpz_poly_derivative(der, pol->coeffs, pol->length);
            _fmpz_poly_evaluate_arb(b, der, pol->length - 1, a, prec);
            _fmpz_vec_clear(der, pol->length - 1);

            if (!arb_contains_zero(b))
            {
                arf_clear(l);
                arf_clear(r);
                arb_clear(b);
                return 1;
            }
        }
        else
            return 0;
        arb_clear(b);

        /* 2 - expensive testing                                        */
        fmpq_t ql, qr;

        fmpq_init(ql);
        fmpq_init(qr);
        arf_get_fmpq(ql, l);
        arf_get_fmpq(qr, r);

        fmpz_poly_init(pol2);
        fmpz_poly_fit_length(pol2, pol->length);
        _fmpz_vec_set(pol2->coeffs, pol->coeffs, pol->length);
        pol2->length = pol->length;
        _fmpz_poly_scale_0_1_fmpq(pol2->coeffs, pol2->length, ql, qr);

        n = fmpz_poly_num_real_roots_0_1(pol2);

        fmpz_poly_clear(pol2);
        fmpq_clear(ql);
        fmpq_clear(qr);

        return (n == 1);
    }
}
