/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <flint/fmpq.h>
#include "e-antic/renfxx.h"

void check_equal(renf_elem_class& a, renf_elem_class& b)
{
    if (a != b)
    {
        std::cout << "a = " << a << std::endl;
        std::cout << "b = " << b << std::endl;
        throw 10;
    }
}

void check_equal(renf_elem_class& a, slong b)
{
    if (a != b)
    {
        std::cout << "a = " << a << std::endl;
        std::cout << "b = " << b << std::endl;
        throw 10;
    }
}


int main(void)
{
    FLINT_TEST_INIT(state);

    {
        renf_elem_class a;
        renf_elem_class b((int) 1);
        renf_elem_class c((unsigned int) 2);
        renf_elem_class d((long) 3);
        renf_elem_class e((unsigned long) 4);
        renf_elem_class f(mpz_class(5));
        renf_elem_class g(mpq_class(6));

        std::cout << "c++ constructor....\n";

        if (not (a.is_fmpq() && b.is_fmpq() && c.is_fmpq() &&
                 d.is_fmpq() && e.is_fmpq() && f.is_fmpq() &&
                 g.is_fmpq()))
        {
            std::cerr << "Problem with integer constructors\n";
            throw 10;
        }

        check_equal(a, 0);
        check_equal(b, 1);
        check_equal(c, 2);
        check_equal(d, 3);
        check_equal(e, 4);
        check_equal(f, 5);
        check_equal(g, 6);
    }

    {
        renf_t nf;
        renf_randtest(nf, state, 10, 50);
        renf_class K(nf);

        renf_elem_class a(K);
        renf_elem_class b(a);
        renf_elem_class c = K.zero();

        if (a.is_fmpq() || b.is_fmpq() || c.is_fmpq())
        {
            std::cerr << "Problem with renf_elem_class constructor" << std::endl;
            throw 10;
        }
        if (not (renf_elem_is_zero(a.get_renf_elem(), nf) &&
                 renf_elem_is_zero(b.get_renf_elem(), nf) &&
                 renf_elem_is_zero(c.get_renf_elem(), nf)))
        {
            std::cerr << "Problem: elements initialized to nonzero values" << std::endl;
            throw 10;
        }

        renf_clear(nf);
    }

    {
        renf_class K("3  -2 0 1", "1.4142 +/- 0.5", 128);
        renf_elem_class a(K);
        renf_elem_class b(K, 0);
        renf_elem_class c(K, "0");
        renf_elem_class d(K, "0 0");
        renf_elem_class e = K.zero();

        if (a.is_fmpq() || b.is_fmpq() || c.is_fmpq() || d.is_fmpq() || e.is_fmpq())
        {
            std::cerr << "Problem with renf_elem_class constructor\n";
            throw 10;
        }

        check_equal(a, b);
        check_equal(a, c);
        check_equal(a, d);
        check_equal(a, e);
    }

    {
        // QQ
        renf_class K;
        renf_elem_class a(K);
        renf_elem_class b(K, 0);
        renf_elem_class c(K, "0");
        renf_elem_class d = K.zero();

        check_equal(a, b);
        check_equal(a, c);
        check_equal(a, d);
    }

    {
        // QQ[sqrt(2)]
        renf_class K("minpoly a^2 - 2 embedding [1.4142 +/- 0.5]", 128);

        fmpq_poly_t p;
        fmpq_poly_init(p);
        fmpq_poly_set_str(p, "2  -7/3 3/5");
        renf_elem_class a(K, p);
        fmpq_poly_clear(p);

        renf_elem_class b(K, "-7/3 3/5");

        renf_elem_class c = 3 * K.gen() / 5 - 7 * K.one() / 3;

        check_equal(a, b);
        check_equal(a, c);
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}
