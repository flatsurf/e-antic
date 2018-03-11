/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renf.h>
#include <e-antic/poly_extra.h>

void check_renf(renf_t nf)
{
    arb_ptr a;
    arb_t b;

    a = nf->emb;

    if (fmpq_poly_length(nf->nf->pol) != fmpz_poly_length(nf->der) + 1)
    {
        printf("FAIL:\n");
        printf("uninitalized derivative");
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
        abort();
    }
    arb_clear(b);
}

int main()
{

    printf("init....");

    /* x^2 - 2 */
    {
        fmpq_poly_t p;
        arb_t emb;
        renf_t nf;

        fmpq_poly_init(p);
        arb_init(emb);

        fmpq_poly_set_coeff_si(p, 0, -2);
        fmpq_poly_set_coeff_si(p, 2, 1);
        arb_set_d(emb, 1.414213562373095);
        arb_add_error_2exp_si(emb, -20);
        renf_init(nf, p, emb, 64);
        check_renf(nf);

        renf_clear(nf);
        fmpq_poly_clear(p);
        arb_clear(emb);
    }

    /* x^4 - 100*x^3 + 50*x^2 - 20*x + 1 */
    {
        fmpq_poly_t p;
        arb_t emb;
        renf_t nf;

        fmpq_poly_init(p);
        arb_init(emb);

        fmpq_poly_zero(p);
        fmpq_poly_set_coeff_si(p, 4, 1);
        fmpq_poly_set_coeff_si(p, 3, -100);
        fmpq_poly_set_coeff_si(p, 2, 50);
        fmpq_poly_set_coeff_si(p, 1, -20);
        fmpq_poly_set_coeff_si(p, 0, 1);
        arb_set_d(emb, 99.4995040886108);
        arb_add_error_2exp_si(emb, -20);
        renf_init(nf, p, emb, 64);
        check_renf(nf);

        renf_clear(nf);
        fmpq_poly_clear(p);
        arb_clear(emb);

    }

    /* p = x^30 - 3000*x - 1 */
    {
        fmpq_poly_t p;
        arb_t emb;
        renf_t nf;

        fmpq_poly_init(p);
        arb_init(emb);

        fmpq_poly_zero(p);
        fmpq_poly_set_coeff_si(p, 30, 1);
        fmpq_poly_set_coeff_si(p, 1, -3000);
        fmpq_poly_set_coeff_si(p, 0, -1);
        arb_set_d(emb, 1.317966950615985);
        arb_add_error_2exp_si(emb, -20);
        renf_init(nf, p, emb, 64);
        check_renf(nf);

        renf_clear(nf);
        fmpq_poly_clear(p);
        arb_clear(emb);
    }

    printf("PASS\n");

    return 0;
}
