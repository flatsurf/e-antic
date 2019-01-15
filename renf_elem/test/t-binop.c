/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renf_elem.h>

int main()
{
    int iter;
    FLINT_TEST_INIT(state);

    for (iter = 0; iter < 100; iter++)
    {
        /* operations on two renf elements */
        renf_t nf;
        slong len = 2 + n_randint(state, 10);
        mp_bitcnt_t bits = 10 + n_randint(state, 30);

        renf_randtest(nf, state, len, bits);

        {
            /* renf renf operations */
            renf_elem_t a, b, c;
            renf_elem_init(a, nf);
            renf_elem_init(b, nf);
            renf_elem_init(c, nf);

            renf_elem_randtest(a, state, 10 + n_randint(state, 30), nf);
            renf_elem_randtest(b, state, 10 + n_randint(state, 30), nf);

            renf_elem_add(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            renf_elem_sub(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            renf_elem_mul(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            if (!renf_elem_is_zero(b, nf))
            {
                renf_elem_div(c, a, b, nf);
                renf_elem_check_embedding(c, nf, 1024);
            }

            renf_elem_clear(a, nf);
            renf_elem_clear(b, nf);
            renf_elem_clear(c, nf);
        }

        {
            /* renf si operations */
            renf_elem_t a, c;
            slong b;
            renf_elem_init(a, nf);
            renf_elem_init(c, nf);

            renf_elem_randtest(a, state, 10 + n_randint(state, 30), nf);
            b = n_randtest(state);

            renf_elem_add_si(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            renf_elem_sub_si(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            renf_elem_mul_si(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            if (b != 0)
            {
                renf_elem_div_si(c, a, b, nf);
                renf_elem_check_embedding(c, nf, 1024);
            }

            renf_elem_clear(a, nf);
            renf_elem_clear(c, nf);
        }

        {
            /* renf ui operations */
            renf_elem_t a, c;
            ulong b;
            renf_elem_init(a, nf);
            renf_elem_init(c, nf);

            renf_elem_randtest(a, state, 10 + n_randint(state, 30), nf);
            b = n_randtest(state);

            renf_elem_add_ui(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            renf_elem_sub_ui(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            renf_elem_mul_ui(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            if (b != 0)
            {
                renf_elem_div_ui(c, a, b, nf);
                renf_elem_check_embedding(c, nf, 1024);
            }

            renf_elem_clear(a, nf);
            renf_elem_clear(c, nf);
        }

        {
            /* renf fmpz operations */
            renf_elem_t a, c;
            fmpz_t b;

            renf_elem_init(a, nf);
            fmpz_init(b);
            renf_elem_init(c, nf);

            renf_elem_randtest(a, state, 10 + n_randint(state, 30), nf);
            fmpz_randtest(b, state, 10 + n_randint(state, 30));

            renf_elem_add_fmpz(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            renf_elem_sub_fmpz(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            renf_elem_mul_fmpz(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            if (!fmpz_is_zero(b))
            {
                renf_elem_div_fmpz(c, a, b, nf);
                renf_elem_check_embedding(c, nf, 1024);
            }

            renf_elem_clear(a, nf);
            fmpz_clear(b);
            renf_elem_clear(c, nf);

        }

        {
            /* renf fmpq operations */
            renf_elem_t a, c;
            fmpq_t b;

            renf_elem_init(a, nf);
            fmpq_init(b);
            renf_elem_init(c, nf);

            renf_elem_randtest(a, state, 10 + n_randint(state, 30), nf);
            fmpq_randtest(b, state, 10 + n_randint(state, 30));

            renf_elem_add_fmpq(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            renf_elem_sub_fmpq(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            renf_elem_mul_fmpq(c, a, b, nf);
            renf_elem_check_embedding(c, nf, 1024);
            if (!fmpq_is_zero(b))
            {
                renf_elem_div_fmpq(c, a, b, nf);
                renf_elem_check_embedding(c, nf, 1024);
            }

            renf_elem_clear(a, nf);
            fmpq_clear(b);
            renf_elem_clear(c, nf);
        }


        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state);

    return 0;
}

