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
void check_eq(T t, renf_t nf)
{
    renf_elem_class a(t);

    if (t != a || a != t || not (a == t) || not (t == a))
        throw 10;

    renf_elem_class b(nf);
    b = t;

    if (t != b || b != t || not (b == t) || not (t == b))
        throw 10;
}

template<typename T>
void check_cmp(T t, renf_t nf)
{
    renf_elem_class a(nf);
    renf_elem_gen(a.get_renf_elem(), nf);

    if (a == t || t == a ||
        not (a != t) || not (t != a) ||
        (a < t) != (t > a) ||
        (a <= t) != (t >= a) ||
        (a > t) != (t < a) ||
        (a >= t) != (t <= a) ||
        (a < t) != not (a >= t) ||
        (a > t) != not (a <= t))
        throw 10;
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

    for (iter = 0; iter < 10; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 5, 10);

        check_eq(c1, nf);
        check_cmp(c1, nf);

        check_eq(c2, nf);
        check_cmp(c2, nf);

        check_eq(c3, nf);
        check_cmp(c3, nf);

        check_eq(c4, nf);
        check_cmp(c4, nf);

        check_eq(c5, nf);
        check_cmp(c5, nf);

        check_eq(c6, nf);
        check_cmp(c6, nf);

        renf_clear(nf);
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}
