/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

#define CLEANUP \
    arb_set_interval_arf(res, l, r, prec); \
    arf_clear(l); \
    arf_clear(r); \
    arf_clear(m); \
    arb_clear(al); \
    arb_clear(bl); \
    arb_clear(ar); \
    arb_clear(br);

void _fmpz_poly_bisection_step_arb(arb_t res, const fmpz * pol, slong len, arb_t a, slong prec)
{
    arb_t al, ar, bl, br;
    arf_t l,r,m;

    arf_init(l);
    arf_init(r);
    arf_init(m);
    arb_init(al);
    arb_init(bl);
    arb_init(ar);
    arb_init(br);

    arb_get_interval_arf(l, r, a, prec);
    printf("a = "); arb_printd(a,20); printf("\n");
    do
    {
#ifdef DEBUG
        printf("l = "); arf_printd(l,20); printf("\n");
        printf("r = "); arf_printd(r,20); printf("\n");
#endif

        arf_add(m, l, r, prec, ARF_RND_NEAR);
        arf_div_si(m, m, 2, prec, ARF_RND_NEAR);
#ifdef DEBUG
        printf("m = "); arf_printd(m,20); printf("\n");
#endif

        if ((arf_cmp(l,m) >= 0) || (arf_cmp(r,m) <= 0))
        {
#ifdef DEBUG
            printf("step 1\n");
#endif
            CLEANUP;
            return;
        }

        arb_set_interval_arf(al, l, m, prec);
        _fmpz_poly_evaluate_arb(bl, pol, len, al, prec);
        arb_set_interval_arf(ar, m, r, prec);
        _fmpz_poly_evaluate_arb(br, pol, len, ar, prec);
#ifdef DEBUG
        printf("bl = "); arb_printd(bl,20); printf("\n");
        printf("br = "); arb_printd(br,20); printf("\n");

        printf("bl contains zero: %d\n", arb_contains_zero(bl));
        printf("br contains zero: %d\n", arb_contains_zero(br));
#endif

        if (arb_contains_zero(bl))
        {
            if (!arb_contains_zero(br))
                arf_swap(r, m);
            else
            {
                /* the precision is not enough to choose the next subinterval */
                CLEANUP;
                return;
            }
        }
        else
        {
            if (arb_contains_zero(br))
                arf_swap(l, m);
            else
            {
                fprintf(stderr, "PROBLEM\n");
                CLEANUP;
                return;
            }
         }
    } while (1);

    CLEANUP;
}

#undef CLEANUP

int fmpz_poly_bisection_step_arb(arb_t res, const fmpz_poly_t pol, arb_t a, slong prec)
{
    int ans;
    arb_t rres;

    if(a == res) arb_init(rres);
    else arb_swap(res, rres);

#ifdef DEBUG
    printf("[fmpz_poly_bisection_step_arb]: before a = "); arb_printd(a,prec); printf("\n");
#endif

    _fmpz_poly_bisection_step_arb(rres, pol->coeffs, fmpz_poly_length(pol), a, prec);

#ifdef DEBUG
    printf("[fmpz_poly_bisection_step_arb]: after a = "); arb_printd(a,prec); printf("\n");
    printf("[fmpz_poly_bisection_step_arb]: after rres = "); arb_printd(rres,prec); printf("\n");
#endif

    ans = arb_contains(a, rres) && !arb_equal(rres, a);

    if (ans)
    {
        arb_swap(res, rres);
        if (a == res) arb_clear(rres);
    }
    else
    {
        if (a == rres) arb_clear(res);
    }

    return ans;
}
