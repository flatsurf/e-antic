/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renfxx.h>
#include <flint/fmpq.h>

int main(void)
{
    renf_elem_class a;
    renf_elem_class b((int) 1);
    renf_elem_class c((unsigned int) 2);
    renf_elem_class d((long) 3);
    renf_elem_class e((unsigned long) 4);

    FLINT_TEST_INIT(state);

    if (not (a.is_fmpq() && b.is_fmpq() && c.is_fmpq() && d.is_fmpq() && e.is_fmpq()))
    {
        std::cerr << "Problem with integer constructors\n";
        throw 10;
    }

    if (fmpq_cmp_ui(a.get_fmpq(), 0) || fmpq_cmp_ui(b.get_fmpq(), 1) || fmpq_cmp_ui(c.get_fmpq(), 2) ||
        fmpq_cmp_ui(d.get_fmpq(), 3) || fmpq_cmp_ui(e.get_fmpq(), 4))
    {
        std::cerr << "renf_elem_class gets constructed with wrong values\n";
        std::cerr << "a = " << a << "\n";
        std::cerr << "b = " << b << "\n";
        std::cerr << "c = " << c << "\n";
        std::cerr << "d = " << d << "\n";
        std::cerr << "e = " << e << "\n";
        throw 10;
    }

    renf_t nf;
    renf_randtest(nf, state, 10, 50);

    renf_elem_class f(nf);
    renf_elem_class g(f);

    if (f.is_fmpq() || g.is_fmpq())
    {
        std::cerr << "Problem with renf_elem_class constructor\n";
        throw 10;
    }
    if (not (renf_elem_is_zero(f.get_renf_elem(), nf) && renf_elem_is_zero(g.get_renf_elem(), nf)))
    {
        std::cerr << "Problem: elements initialized to nonzero values\n";
        throw 10;
    }

    renf_clear(nf);

    FLINT_TEST_CLEANUP(state);
    return 0;
}
