/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>

#include <e-antic/renf_elem.h>

int check_approx(renf_elem_t a, renf_t nf, double down, double up, double best)
{
    int sgn;

    if (renf_elem_get_d(a, nf, ARF_RND_DOWN) != down ||
        renf_elem_get_d(a, nf, ARF_RND_UP) != up ||
        renf_elem_get_d(a, nf, ARF_RND_NEAR) != best)
    {
        fprintf(stderr, "wrong DOWN/UP/NEAR\n");
        fprintf(stderr, "down: got %.30f and expect %.30f\n",
                    renf_elem_get_d(a, nf, ARF_RND_DOWN),
                    down);
        fprintf(stderr, "up  : got %.30f and expect %.30f\n",
                    renf_elem_get_d(a, nf, ARF_RND_UP),
                    up);
        fprintf(stderr, "near: got %.30f and expect %.30f\n",
                    renf_elem_get_d(a, nf, ARF_RND_NEAR),
                    best);
        fprintf(stderr, "wrong DOWN/UP/NEAR\n");
        fprintf(stderr, "wrong DOWN/UP/NEAR\n");
        return 1;
    }

    sgn = renf_elem_sgn(a, nf);
    if (sgn == 0)
    {
        if (renf_elem_get_d(a, nf, ARF_RND_FLOOR) != 0 ||
            renf_elem_get_d(a, nf, ARF_RND_CEIL) != 0)
        {
            fprintf(stderr, "wrong FLOOR/CEIL\n");
            return 1;
        }
    }
    else if (sgn > 0)
    {
        if (renf_elem_get_d(a, nf, ARF_RND_FLOOR) != down ||
            renf_elem_get_d(a, nf, ARF_RND_CEIL) != up)
        {
            fprintf(stderr, "wrong FLOOR/CEIL\n");
            return 1;
        }
    }
    else
    {
        if (renf_elem_get_d(a, nf, ARF_RND_FLOOR) != up ||
            renf_elem_get_d(a, nf, ARF_RND_CEIL) != down)
        {
            fprintf(stderr, "wrong FLOOR/CEIL\n");
            return 1;
        }
    }

    return 0;
}

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    renf_t nf;

    /* initialize nf Q[(1+sqrt(5)/2] */
    {
        fmpq_poly_t p;
        arb_t emb;

        fmpq_poly_init(p);

        fmpq_poly_set_coeff_si(p, 2, 1);
        fmpq_poly_set_coeff_si(p, 1, -1);
        fmpq_poly_set_coeff_si(p, 0, -1);

        arb_init(emb);
        arb_set_d(emb, 1.61803398874989);
        arb_add_error_2exp_si(emb, -20);
        renf_init(nf, p, emb, 20 + n_randint(state, 100));
        fmpq_poly_clear(p);
        arb_clear(emb);
    }

    /* zero test */
    {
        renf_elem_t a;
        renf_elem_init(a, nf);

        renf_elem_zero(a, nf);
        if (check_approx(a, nf, 0, 0, 0))
        {
            fprintf(stderr, "test0 failed\n");
            return 1;
        }

        renf_elem_clear(a, nf);
    }

    /* generator (1+sqrt(5))/2 */
    {
        renf_elem_t a;
        double down = ldexp(0X19e3779b97f4a7, -52);
        double up = ldexp(0X19e3779b97f4a8, -52);
        double best = up;

        renf_elem_init(a, nf);
        renf_elem_gen(a, nf);

        if (check_approx(a, nf, down, up, best))
        {
            fprintf(stderr, "test1 failed\n");
            return 1;
        }
        renf_elem_neg(a, a, nf);
        if (check_approx(a, nf, -down, -up, -best))
        {
            fprintf(stderr, "test2 failed\n");
            return 1;
        }

        renf_elem_clear(a, nf);
    }

    /* fibonacci(30) * phi - fibonacci(31) */
    {
        renf_elem_t a;
        double up = ldexp(-0x1209021bdbabdb, -73);
        double down = ldexp(-0x1209021bdbabda, -73);
        double best = down;

        renf_elem_init(a, nf);
        renf_elem_gen(a, nf);
        renf_elem_mul_si(a, a, 832040, nf);
        renf_elem_sub_si(a, a, 1346269, nf);

        if (check_approx(a, nf, down, up, best))
        {
            fprintf(stderr, "test3 failed\n");
            return 1;
        }

        renf_elem_clear(a, nf);
    }

    renf_clear(nf);

    FLINT_TEST_CLEANUP(state);
    return 0;
}

