/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "poly_extra.h"
#include "arb.h"

static __inline__ int
arb_sgn2(arb_t a)
{
    if (arb_is_positive(a)) return 1;
    else if (arb_is_negative(a)) return -1;
    return 0;
}

int fmpq_poly_check_unique_real_root(const fmpq_poly_t pol, const arb_t a, slong prec)
{
    arb_t b, c;
    arf_t l, r;
    fmpq_t ql, qr;
    fmpz * der;
    int lsign, rsign;
    fmpz_poly_t pol2;
    slong n;

#ifdef DEBUG
    printf("[fmpq_poly_check_unique_real_root] pol = "); fmpq_poly_print(pol); printf("\n");
    printf("[fmpq_poly_check_unique_real_root] a = "); arb_printd(a, 10); printf("\n");
    fflush(stdout);
#endif

    if(pol->length < 2)
        return 0;

    /* 1 - cheap test:                    */
    /*   - sign(left) * sign(right) = -1  */
    /*   - no zero of the derivative      */
#ifdef DEBUG
    printf("[fmpq_poly_check_unique_real_root] cheap test\n");
    fflush(stdout);
#endif

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

    /* 2 - expensive test                                          */
    /* enclose a in an interval of the form [c / 2^k, (c + 1)/2^k] */
    /* then call the algorithm to find roots in [0, 1]             */
#ifdef DEBUG
    printf("[fmpq_poly_check_unique_real_root] expensive test\n");
    fflush(stdout);
#endif
    arb_clear(b);

    fmpq_init(ql);
    fmpq_init(qr);
    arf_get_fmpq(ql, l);
    arf_get_fmpq(qr, r);

#ifdef DEBUG
    printf("[fmpq_poly_check_unique_real_root] ql = "); fmpq_print(ql); printf("\n");
    printf("[fmpq_poly_check_unique_real_root] qr = "); fmpq_print(qr); printf("\n");
    fflush(stdout);
#endif

    fmpz_poly_init(pol2);
    fmpz_poly_fit_length(pol2, pol->length);
    _fmpz_vec_set(pol2->coeffs, pol->coeffs, pol->length);
    pol2->length = pol->length;
    _fmpz_poly_scale_0_1_fmpq(pol2->coeffs, pol2->length, ql, qr);
#ifdef DEBUG
    flint_printf("[fmpq_poly_check_unique_real_root] scaled poly = "); fmpz_poly_print(pol2); printf("\n");
    flint_printf("[fmpq_poly_check_unique_real_root] vca count = %wd\n",
            fmpz_poly_num_real_roots_0_1_vca(pol2));
    flint_printf("[fmpq_poly_check_unique_real_root] Sturm count = %wd\n",
            fmpz_poly_num_real_roots_0_1_sturm(pol2));
    fflush(stdout);
#endif

    n = fmpz_poly_num_real_roots_0_1(pol2);
    fmpz_poly_clear(pol2);

    fmpq_clear(ql);
    fmpq_clear(ql);

    return (n == 1);
}
