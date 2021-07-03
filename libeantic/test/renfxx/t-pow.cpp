/*
    Copyright (C)      2019 Vincent Delecroix
                  2019-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <limits.h>

#include "../../e-antic/renfxx.h"

using namespace eantic;

int main(void)
{
    FLINT_TEST_INIT(state);
    int iter;

    for (iter = 0; iter < 100; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 10, 64, 10);
        auto K = renf_class::make(nf);
        renf_clear(nf);

        {
            renf_elem_class a(*K, 2);
            if (a.pow(2) != 4)
                throw std::runtime_error("2^2 is wrong");
            if (4 * a.pow(-2) != 1)
                throw std::runtime_error("2^-2 is wrong");
        }

        {
            renf_elem_class a = K->gen();
            if (a.pow(5) != pow(a, 5))
                throw std::runtime_error("a.pow() and pow(a) disagree");
            if (a.pow(5) * a.pow(-5) != 1)
                throw std::runtime_error("a^5 is wrong");
        }
    }

}

