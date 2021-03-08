/*
    Copyright (C) 2018 Vincent Delecroix
                  2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renfxx.h"

using namespace eantic;

int main(void)
{
    FLINT_TEST_INIT(state);

    {
        renf_elem_class a(2);

        // should work
        a.get_renf_elem();
    }

    {
        renf_t nf;
        renf_randtest(nf, state, 5, 64, 50);
        const auto K = renf_class::make(nf);
        renf_clear(nf);

        renf_elem_class a(*K);

        // should work
        a.get_renf_elem();
    }

    FLINT_TEST_CLEANUP(state)
    return 0;
}

