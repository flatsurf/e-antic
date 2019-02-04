/*=============================================================================

    This file is part of FLINT.

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

    flint_printf("mul_gen....");
    fflush(stdout);

    flint_randinit(state);

    /* test mul_gen(b) = a * b, where a is the generator */
    for (i = 0; i < 100; i++)
    {
        nf_t nf;
        nf_elem_t a, b, p1, p2;

        nf_init_randtest(nf, state, 40, 200);
        while (nf_degree(nf) == 1)
        {
            nf_clear(nf);
            nf_init_randtest(nf, state, 40, 200);
        }
        
        nf_elem_init(a, nf);
        nf_elem_init(b, nf);
        nf_elem_init(p1, nf);
        nf_elem_init(p2, nf);

        nf_elem_gen(a, nf);

        nf_elem_randtest(b, state, 200, nf);

        nf_elem_mul_gen(p1, b, nf);
        nf_elem_mul(p2, b, a, nf);

        result = (nf_elem_equal(p1, p2, nf));
        if (!result)
        {
           printf("FAIL:\n");
           printf("K = "); nf_print(nf); printf("\n");
           printf("a = "); nf_elem_print_pretty(a, nf, "x"); printf("\n");
           printf("b = "); nf_elem_print_pretty(b, nf, "x"); printf("\n");
           printf("p1 = "); nf_elem_print_pretty(p1, nf, "x"); printf("\n");
           printf("p2 = "); nf_elem_print_pretty(p2, nf, "x"); printf("\n");
           abort();
        }

        nf_elem_clear(a, nf);
        nf_elem_clear(b, nf);
        nf_elem_clear(p1, nf);
        nf_elem_clear(p2, nf);
         
        nf_clear(nf);
    }
    
    /* test aliasing b and b */
    for (i = 0; i < 100; i++)
    {
        nf_t nf;
        nf_elem_t b, c;

        nf_init_randtest(nf, state, 40, 200);
        while (nf_degree(nf) == 1)
        {
            nf_clear(nf);
            nf_init_randtest(nf, state, 40, 200);
        }


        nf_elem_init(b, nf);
        nf_elem_init(c, nf);

        nf_elem_randtest(b, state, 200, nf);
        nf_elem_randtest(c, state, 200, nf);
        
        nf_elem_mul_gen(b, c, nf);
        nf_elem_mul_gen(c, c, nf);
        
        result = (nf_elem_equal(b, c, nf));
        if (!result)
        {
           printf("b = "); nf_elem_print_pretty(b, nf, "x"); printf("\n");
           printf("c = "); nf_elem_print_pretty(c, nf, "x"); printf("\n");
           abort();
        }

        nf_elem_clear(b, nf);
        nf_elem_clear(c, nf);
         
        nf_clear(nf);
    }
    
    flint_randclear(state);
    flint_printf("PASS\n");
    return 0;
}
