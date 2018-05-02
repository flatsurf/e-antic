/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/renfxx.h"

template<typename T>
void check_cmp(T t, renf_elem_class& a)
{
    if (a == t || t == a ||
        not (a != t) || not (t != a) ||    
        (a < t) != (t > a) ||
        (a <= t) != (t >= a) ||
        (a > t) != (t < a) ||
        (a >= t) != (t <= a) ||
        (a < t) != not (a >= t) ||
        (a > t) != not (a <= t))
    {
        std::cerr << "contradictions in comparisons";
        exit(EXIT_FAILURE);
    }
}


int main(void)
{
    int c1 = -1123;
    unsigned int c2 = 2223;
    long c3 = 134;
    unsigned long c4 = 513;
    mpz_class c5(232);
    mpq_class c6(211561);
    int iter;
    FLINT_TEST_INIT(state);

    std::cout << "test1\n";
    std::cout.flush();


    if (c1 != renf_elem_class(c1) || renf_elem_class(c1) != c1 ||
        c2 != renf_elem_class(c2) || renf_elem_class(c2) != c2 ||
        c3 != renf_elem_class(c3) || renf_elem_class(c3) != c3 ||
        c4 != renf_elem_class(c4) || renf_elem_class(c4) != c4 ||
        c5 != renf_elem_class(c5) || renf_elem_class(c5) != c5 ||
        c6 != renf_elem_class(c6) || renf_elem_class(c6) != c6)
    {
        std::cerr << "pb with elementary comparisons\n";
        exit(EXIT_FAILURE);
    }

    for (iter = 0; iter < 10; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 5, 10);
        renf_print(nf);

        renf_elem_class a(nf);
        renf_elem_gen(a.get_renf_elem(), nf);

        check_cmp(c1, a);
        check_cmp(c2, a);
        check_cmp(c3, a);
        check_cmp(c4, a);
        check_cmp(c5, a);
        check_cmp(c6, a);

        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}

