/*
    Copyright (C) 2018 Vincent Delecroix
                  2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <stdexcept>

#include "../../e-antic/renfxx.h"

using namespace eantic;

int main(void)
{
    FLINT_TEST_INIT(state);
    ulong iter;

    {
        renf_elem_class a(mpq_class("3/2"));
        if (a.ceil() != 2)
            throw std::runtime_error("pb with ceil");
        if (static_cast<double>(a) != 1.5)
            throw std::runtime_error("double cast wrong");
        if (a.get_d() != 1.5)
            throw std::runtime_error("double cast wrong");
    }
    {
        renf_elem_class a(mpq_class("-3/2"));
        if (a.ceil() != -1)
            throw std::runtime_error("pb with ceil");
        if (static_cast<double>(a) != -1.5)
            throw std::runtime_error("double cast wrong");
        if (a.get_d() != -1.5)
            throw std::runtime_error("double cast wrong");
    }

    {
        fmpq_t k;
        fmpq_poly_t p;

        const auto K = renf_class::make("x^2-x-1", "x", "1.618 +/- 0.1");

        /* (1+sqrt(5))/2 vs Fibonacci */
        renf_elem_class a(*K);

        fmpq_poly_init(p);
        fmpq_init(k);
        fmpq_poly_zero(p);
        fmpq_poly_set_coeff_si(p, 1, -1);
        for (iter = 1; iter < 2000; iter++)
        {
            fmpz_fib_ui(fmpq_numref(k), iter + 1);
            fmpz_fib_ui(fmpq_denref(k), iter);
            fmpq_poly_set_coeff_fmpq(p, 0, k);
            renf_elem_set_fmpq_poly(a.get_renf_elem(), p, K->get_renf());

            if (a.ceil() != ceil(a))
                throw std::runtime_error("a.ceil() and ceil() disagree");
            if (a.ceil() != 1 - iter % 2)
            {
                std::cerr << "pb with Fibonacci ceil" << std::endl;
                std::cerr << "got " << a.ceil() << " instead of " << (1 - iter % 2) << " at iteration " << iter << std::endl;

                throw std::runtime_error("ceil test error");
            }
        }

        fmpq_clear(k);
        fmpq_poly_clear(p);
    }

    FLINT_TEST_CLEANUP(state)
    return 0;
}

