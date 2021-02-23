/*=============================================================================

  This file is part of ANTIC.

  FLINT is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  FLINT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FLINT; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

  =============================================================================*/
/******************************************************************************

  Copyright (C) 2018 Tommy Hofmann

 ******************************************************************************/

#include <stdio.h>
#include "e-antic/nf.h"
#include "e-antic/nf_elem.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    flint_printf("get_fmpz_mod_poly....");
    fflush(stdout);

    flint_randinit(state);

    for (i = 0; i < 100; i++)
    {
        slong j;
        nf_t nf;
        nf_elem_t a;
        fmpz_mod_poly_t reduced_elem;
        fmpz_t coeff, mod, reduced_coeff;
        fmpz_mod_ctx_t ctx;

        fmpz_init(mod);
        fmpz_randtest_unsigned(mod, state, 2 * FLINT_BITS);
        fmpz_add_ui(mod, mod, 2);

        fmpz_init(coeff);
        fmpz_init(reduced_coeff);
        fmpz_mod_ctx_init(ctx, mod);
        fmpz_mod_poly_init(reduced_elem, ctx);

        nf_init_randtest(nf, state, 2 + n_randint(state, 20), 200);

        nf_elem_init(a, nf);

        nf_elem_randtest(a, state, 200, nf);

        nf_elem_get_fmpz_mod_poly_den(reduced_elem, a, nf, 0, ctx);

        for (j = 0; j < nf_degree(nf); j++)
        {
            nf_elem_get_coeff_fmpz(coeff, a, j, nf);
            fmpz_mod(coeff, coeff, mod);
            fmpz_mod_poly_get_coeff_fmpz(reduced_coeff, reduced_elem, j, ctx);
            result = fmpz_equal(reduced_coeff, coeff);
            if (!result)
            {
                printf("FAIL: Reducing without denominator\n");
                printf("f = "); fmpq_poly_print_pretty(nf->pol, "x"); printf("\n");
                printf("a = "); nf_elem_print_pretty(a, nf, "x"); printf("\n");
                printf("n = "); fmpz_print(mod); printf("\n");
                printf("a mod n = "); fmpz_mod_poly_print_pretty(reduced_elem, "x", ctx); printf("\n");
                abort();
            }
        }

        nf_elem_clear(a, nf);
        fmpz_mod_poly_clear(reduced_elem, ctx);
        fmpz_mod_ctx_clear(ctx);
        fmpz_clear(coeff);
        fmpz_clear(reduced_coeff);
        fmpz_clear(mod);
        nf_clear(nf);
    }

    for (i = 0; i < 100; i++)
    {
        slong j;
        nf_t nf;
        nf_elem_t a;
        fmpz_mod_poly_t reduced_elem;
        fmpz_t coeff, reduced_coeff, den, mod, d_mod, d_modinv;
        fmpz_mod_ctx_t ctx;

        fmpz_init(coeff);
        fmpz_init(den);
        fmpz_init(mod);
        fmpz_init(d_mod);
        fmpz_init(d_modinv);
        fmpz_init(reduced_coeff);

        fmpz_init(mod);
        fmpz_randtest_unsigned(mod, state, 2 * FLINT_BITS);
        fmpz_add_ui(mod, mod, 2);

        fmpz_mod_ctx_init(ctx, mod);

        fmpz_mod_poly_init(reduced_elem, ctx);

        nf_init_randtest(nf, state, 40, 200);

        nf_elem_init(a, nf);

        do {
            nf_elem_randtest(a, state, 200, nf);
            nf_elem_get_den(den, a, nf);
            fmpz_mod(d_mod, den, mod);
            fmpz_gcd(d_mod, d_mod, mod);
        } while (!fmpz_is_one(d_mod));

        nf_elem_get_fmpz_mod_poly(reduced_elem, a, nf, ctx);

        for (j = 0; j < nf_degree(nf); j++)
        {
            nf_elem_get_coeff_fmpz(coeff, a, j, nf);
            nf_elem_get_den(den, a, nf);
            fmpz_invmod(d_modinv, den, mod);
            fmpz_mul(coeff, coeff, d_modinv);
            fmpz_mod(coeff, coeff, mod);
            fmpz_mod_poly_get_coeff_fmpz(reduced_coeff, reduced_elem, j, ctx);
            result = (fmpz_equal(coeff, reduced_coeff));
            if (!result)
            {
                printf("FAIL: Reducing element with denominator\n");
                printf("a = "); nf_elem_print_pretty(a, nf, "x"); printf("\n");
                printf("n = "); fmpz_print(mod); flint_printf("\n");
                printf("a mod n = "); fmpz_mod_poly_print_pretty(reduced_elem, "x", ctx); printf("\n");
                abort();
            }
        }

        fmpz_clear(den);
        fmpz_clear(mod);
        fmpz_clear(coeff);
        fmpz_clear(reduced_coeff);
        fmpz_clear(d_mod);
        fmpz_clear(d_modinv);
        nf_elem_clear(a, nf);
        fmpz_mod_poly_clear(reduced_elem, ctx);
        fmpz_mod_ctx_clear(ctx);
        nf_clear(nf);
    }

    flint_randclear(state);
    flint_printf("PASS\n");
    return 0;
}
