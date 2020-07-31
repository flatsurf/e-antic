/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renfxx.h>
#include <iostream>

using namespace eantic;

int main(void)
{
    FLINT_TEST_INIT(state);
    int iter;

    {
        renf_elem_class a(4);
        if (not a.is_integer() || not a.is_rational())
            throw 10;

        if (not (a/2).is_integer() || not (a/2).is_rational())
            throw 10;

        if ((a/3).is_integer() || not (a/3).is_rational())
            throw 10;
    }

    for (iter = 0; iter < 10; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 5, 32, 20);
        auto K = renf_class::make(nf);
        renf_clear(nf);

        if (fmpq_poly_length(nf->nf->pol) <= 1)
            continue;

        renf_elem_class a(K);
        renf_elem_gen(a.get_renf_elem(), K->get_renf());

        if (a.is_integer() || a.is_rational())
            throw 10;

        if (not (a-a+1).is_integer() || not (a-a+1).is_rational())
            throw 10;

        if (((a-a+1)/2).is_integer() || not ((a-a+1)/2).is_rational())
            throw 10;
    }

    FLINT_TEST_CLEANUP(state)
    return 0;
}
