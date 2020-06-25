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

    Copyright (C) 2019 Vincent Delecroix

******************************************************************************/

#include <stdio.h>
#include <antic/nf.h>
#include "../../upstream/patched/nf.h"
#include "e-antic/nf_elem.h"

int
main(void)
{
    int i;
    FLINT_TEST_INIT(state);
    flint_printf("get_str_pretty....");

    for (i = 0; i < 100; i++)
    {
        char * s;
        nf_t nf;
        nf_elem_t a;

        nf_init_randtest(nf, state, 10, 200);
        nf_elem_init(a, nf);
        nf_elem_randtest(a, state, 200, nf);

        s = nf_elem_get_str_pretty(a, "x", nf);
        flint_free(s);
        s = nf_elem_get_str_pretty(a, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", nf);
        flint_free(s);

        nf_elem_clear(a, nf);
        nf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state);
    flint_printf("PASS\n");
    return 0;
}
