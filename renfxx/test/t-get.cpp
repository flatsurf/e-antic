/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renfxx.h>

using namespace eantic;

int main(void)
{
    FLINT_TEST_INIT(state);

    {
        fmpq * q;
        renf_elem_srcptr r;

        renf_elem_class a(2);

        // should work
        q = a.get_fmpq();

        // should not work
        try
        {
            r = a.get_renf_elem();
            throw std::runtime_error("get_renf_elem worked but shouldn't!");
        }
        catch (std::invalid_argument)
        {
        }
    }

    {
        fmpq * q;
        renf_elem_srcptr r;

        renf_t nf;
        renf_randtest(nf, state, 5, 64, 50);
        auto K = renf_class::make(nf);
        renf_clear(nf);

        renf_elem_class a(K);

        // should work
        r = a.get_renf_elem();

        // should not work
        try
        {
            q = a.get_fmpq();
            throw std::runtime_error("get_fmpq worked but shouldn't!");
        }
        catch (std::invalid_argument)
        {
        }
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}

