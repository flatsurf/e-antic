/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renf_elem.h"

int main()
{
    renf_t nf;
    fmpq_t d;
    FLINT_TEST_INIT(state);

    fmpq_init(d);

    fmpq_set_si(d, 3, 1);
    renf_init_nth_root_fmpq(nf, d, 4, 20 + (slong)n_randint(state, 30));

    {
        renf_elem_t a;
        fmpq_t b;
        fmpq_init(b);
        fmpq_set_si(b, -10, 7);
        renf_elem_init(a, nf);
        renf_elem_set_fmpq(a, b, nf);
        if (!renf_elem_equal_fmpq(a, b, nf))
        {
            fprintf(stderr, "not equal\n");
            return 1;
        }

        renf_elem_add_si(a, a, 1, nf);
        if (renf_elem_equal_fmpq(a, b, nf))
        {
            fprintf(stderr, "equal\n");
            return 1;
        }

        renf_elem_clear(a, nf);
        fmpq_clear(b);
    }

    fmpq_clear(d);
    renf_clear(nf);

    FLINT_TEST_CLEANUP(state)
    return 0;
}
