/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renf.h"
#include "../../e-antic/fmpz_poly_extra.h"
#include "../../e-antic/fmpq_poly_extra.h"

static void check_renf(renf_t nf)
{
    arb_ptr a;
    arb_t b;

    a = nf->emb;

    if (fmpq_poly_length(nf->nf->pol) != fmpz_poly_length(nf->der) + 1)
    {
        printf("FAIL:\n");
        printf("uninitalized derivative");
        fflush(stdout);
        abort();
    }

    arb_init(b);
    fmpq_poly_evaluate_arb(b, nf->nf->pol, a, nf->prec);
    if (!arb_contains_zero(b))
    {
        printf("FAIL:\n");
        printf("evaluation does not contain zero\n");
        printf("pol = "); fmpq_poly_print_pretty(nf->nf->pol, "x"); printf("\n");
        printf("a = "); arb_printd(a, 10); printf("\n");
        printf("b = "); arb_printd(b, 10); printf("\n");
        fflush(stdout);
        abort();
    }
    arb_clear(b);
}

/* set res to a ball that :            */
/*     - contains emb +/- rad1         */
/*     - is contained in emb +/- rad 2 */
static void randomized_embedding(arb_t res, double emb, double rad1, double rad2)
{
    double t; /* in +/- (rad2 - rad1) / 2 */
    double r; /* in [rad1 + |t|, rad2 - |t|]*/

    if (rad1 <= 0 || rad2 <= 0 || rad1 >= rad2)
        abort();

    t = (rad1 - rad2) / 2 + (rand() * (rad2 - rad1)) / RAND_MAX;

    if (2 * fabs(t) > (rad2 - rad1))
        abort();

    r = (rad1 + fabs(t)) + (rand() * (rad2 - rad1 - 2 * fabs(t))) / RAND_MAX;
    if (r < rad1 + fabs(t) || r > rad2 - fabs(t))
        abort();

    arf_set_d(arb_midref(res), emb + t);
    mag_set_d(arb_radref(res), r);
}

static void check_init(fmpq_poly_t p, double demb, double rad1, double rad2, slong iter, flint_rand_t state)
{
    renf_t nf;
    arb_t emb;
    slong prec, i;

    arb_init(emb);
    for (i = 0; i < iter; i++)
    {
        prec = 8 + (slong)n_randint(state, 2048);
        randomized_embedding(emb, demb, rad1, rad2);

        renf_init(nf, p, emb, prec);
        check_renf(nf);
        renf_clear(nf);
    }
    arb_clear(emb);
}

int main()
{
    fmpq_poly_t p;

    FLINT_TEST_INIT(state);

    fmpq_poly_init(p);

    /* x^2 - 2 */
    /* two roots: 1.4142 and -14142 */
    fmpq_poly_set_str_pretty(p, "x^2-2", "x");
    check_init(p, -1.414213562373095, 1e-15, 1, 100, state);
    check_init(p, 1.414213562373095, 1e-15, 1, 100, state);

    /* x^4 - 100*x^3 + 50*x^2 - 20*x + 1 */
    /* two roots: 0.057258 and 99.4995 */
    fmpq_poly_set_str_pretty(p, "x^4-100*x^3+50*x^2-20*x+1", "x");
    check_init(p, 99.4995040886108, 1e-13, 80, 100, state);
    check_init(p, 0.0572581811668313022, 1e-16, 80, 100, state);

    /* p = x^30 - 3000*x - 1 */
    /* two roots: -0.00033 and 1.31796 */
    fmpq_poly_set_str_pretty(p, "x^30-3000*x-1", "x");
    check_init(p, -0.00033333333333333333333, 1e-16, 1, 100, state);
    check_init(p, 1.3179669506159841597, 1e-15, 1, 100, state);

    /* p = 2606*x^9 + 770*x^8 - 519*x^7 - 680*x^6 + 2474*x^5 + 1088*x^4 + 28*x^3 - 2299*x^2 + 1973*x - 319 */
    /* unique root: 0.212556 */
    fmpq_poly_set_str_pretty(p, "2606*x^9+770*x^8-519*x^7-680*x^6+2474*x^5+1088*x^4+28*x^3-2299*x^2+1973*x-319", "x");
    check_init(p, 0.212556143628484178, 1e-16, 1000, 100, state);

    /* p = -1705*x^2-7650*x-3297 */
    /* two roots: -4.0038 and -0.48296 */
    fmpq_poly_set_str_pretty(p, "-1705*x^2-7650*x-3297", "x");
    check_init(p, -4.0038355454170714287, 1e-15, 3, 100, state);
    check_init(p, -0.48296797364451214901, 1e-15, 3, 100, state);

    fmpq_poly_clear(p);
    FLINT_TEST_CLEANUP(state)

    return 0;
}
