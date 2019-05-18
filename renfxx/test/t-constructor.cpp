/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <flint/fmpq.h>
#include <e-antic/renfxx.h>

using namespace eantic;

void check_equal(renf_elem_class& a, renf_elem_class& b)
{
    if (a != b)
    {
        std::cerr << "a = " << a.get_str(EANTIC_STR_ALG | EANTIC_STR_D) << "\n";
        std::cerr << "b = " << b.get_str(EANTIC_STR_ALG | EANTIC_STR_D) << "\n";
        std::cerr.flush();

        throw std::runtime_error("a and b expected equal");
    }
}

void check_equal(renf_elem_class& a, slong b)
{
    if (a != b)
    {
        std::cerr << "a = " << a.get_str(EANTIC_STR_ALG | EANTIC_STR_D) << "\n";
        std::cerr << "b = " << b << "\n";
        std::cerr.flush();

        throw std::runtime_error("a and b expected equal");
    }
}


int main(void)
{
    FLINT_TEST_INIT(state);

    {
        renf_elem_class a;
        renf_elem_class b((int) 1);
        renf_elem_class c(mpz_class(2));
        renf_elem_class d(mpq_class(3));

        if (not (a.is_fmpq() && b.is_fmpq() && c.is_fmpq() &&
                 d.is_fmpq()))
            throw std::runtime_error("problem with integer constructors");

        check_equal(a, 0);
        check_equal(b, 1);
        check_equal(c, 2);
        check_equal(d, 3);
    }

    {
        // operator = for renf_class
        renf_class K1("a^2 - 2", "a", "1.41 +/- 0.1", 128);
        renf_class K2;
        K2 = K1;

        renf_elem_class a(K1, "a + 1");
        renf_elem_class b(K2, "a + 1");

        if (K1.degree() != 2 || K2.degree() != 2)
            throw std::runtime_error("wrong answer for degree");
    }

    {
        renf_t nf;
        renf_randtest(nf, state, 10, 128, 50);
        renf_class K(nf);

        renf_elem_class a(K);
        renf_elem_class b(a);
        renf_elem_class c = K.zero();

        if (a.is_fmpq() || b.is_fmpq() || c.is_fmpq())
            throw std::runtime_error("problem with renf_elem_class constructor");

        if (not (renf_elem_is_zero(a.get_renf_elem(), nf) &&
                 renf_elem_is_zero(b.get_renf_elem(), nf) &&
                 renf_elem_is_zero(c.get_renf_elem(), nf)))
            throw std::runtime_error("problem: elements initialized to nonzero values");

        renf_clear(nf);
    }

    {
        renf_class K("a^2 - 2", "a", "1.4142 +/- 0.5", 128);
        renf_elem_class a(K);
        renf_elem_class b(K, 0);
        renf_elem_class c(K, "0");
        renf_elem_class d(K, "0*a^2 + 0");
        renf_elem_class e = K.zero();

        if (a.is_fmpq() || b.is_fmpq() || c.is_fmpq() || d.is_fmpq() || e.is_fmpq())
            throw std::runtime_error("problem with renf_elem_class constructor");

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
        renf_class K("a^2 - 2", "a", "1.41 +/- 0.1", 128);

        renf_elem_class a(K, "-7/3 + 3/5*a");
        renf_elem_class b = 3 * K.gen() / 5 - 7 * K.one() / 3;

        check_equal(a, b);
    }

    {
        // QQ[cbrt(2)]
        renf_class K("A^3 - 2", "A", "1.26 +/- 0.1");

        renf_elem_class a(K, "2/5 - 3/2 * A + 7/4 * A^2");
        renf_elem_class b = mpq_class(2, 5) - mpq_class(3, 2) * K.gen() + mpq_class(7, 4) * K.gen() * K.gen();

        check_equal(a, b);
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}
