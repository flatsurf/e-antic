/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>

#include "../../e-antic/renf_elem.h"

int main(void)
{
    fmpq_t k;
    renf_t nf;
    renf_elem_t a;
    fmpq_poly_t pol;
    char * s;
    size_t i, j;
    int flags[5] = {
        EANTIC_STR_ALG,
        EANTIC_STR_D,
        EANTIC_STR_ARB,
        EANTIC_STR_ALG | EANTIC_STR_D,
        EANTIC_STR_ALG | EANTIC_STR_ARB};
    char * output[5] = {
        "-2/3*x+1/2",
        "-0.99071198",
        "[-0.9907119850 +/- 1.43e-13]",
        "-2/3*x+1/2 ~ -0.99071198",
        "-2/3*x+1/2 ~ [-0.9907119850 +/- 1.43e-13]"};
    char * shifted_output [8] = {
        "-9.9071198",
        "-99.071198",
        "-990.71198",
        "-9907.1198",
        "-99071.198",
        "-990711.98",
        "-9907119.8",
        "-9.9071198e+7",
        };

    FLINT_TEST_INIT(state);

    fmpq_init(k);
    fmpq_set_si(k, 5, 1);
    renf_init_nth_root_fmpq(nf, k, 2, 20 + (slong)n_randint(state, 100));
    renf_elem_init(a, nf);

    fmpq_poly_init(pol);
    fmpq_set_si(k, 1, 2);
    fmpq_poly_set_coeff_fmpq(pol, 0, k);
    fmpq_set_si(k, -2, 3);
    fmpq_poly_set_coeff_fmpq(pol, 1, k);

    renf_elem_set_fmpq_poly(a, pol, nf);
    
    fmpq_clear(k);
    fmpq_poly_clear(pol);

    /* flags are EANTIC_STR_ALG, EANTIC_STR_D and EANTIC_STR_ARB */
    for (i = 0; i < 5; i++)
    {
        s = renf_elem_get_str_pretty(a, "x", nf, 10, flags[i]);
        if (strcmp(s, output[i]))
        {
            fprintf(stderr, "wrong output for i=%zu; expected %s got %s\n", i, output[i], s);
            return 1;
        }
        flint_free(s);
    }

    /* Check that large numbers can be printed */
    for (j = 0; j < 1024; j++)
    {
        renf_elem_mul_ui(a, a, 10, nf);

        for (i = 0; i < 5; i++) {
            s = renf_elem_get_str_pretty(a, "x", nf, 10, flags[i]);

            if (i == 0 || i == 3 || i == 4) {
                if (strncmp(s, "-2", 2)) {
                    fprintf(stderr, "wrong output for i=%zu; expected output to start with -2 but found %s\n", i, s);
                    return 1;
                }
            } else if (i == 1) {
                if (strncmp(s, "-9", 2) && strcmp(s, "-inf")) {
                    fprintf(stderr, "wrong output for i=%zu; expected output to be -inf or start with -9 but found %s\n", i, s);
                    return 1;
                }
                if (j < 8) {
                    if (strcmp(s, shifted_output[j])) {
                        fprintf(stderr, "wrong output for i=%zu; expected output to be %s but found %s\n", i, shifted_output[j], s);
                        return 1;
                    }
                }
            } else if (i == 4) {
                if (strncmp(s, "[-9", 3)) {
                    fprintf(stderr, "wrong output for i=%zu; expected output to start with [-9 but found %s\n", i, s);
                    return 1;
                }
            }

            flint_free(s);
        }
    }

    renf_elem_clear(a, nf);
    renf_clear(nf);

    FLINT_TEST_CLEANUP(state)
    return 0;
}

