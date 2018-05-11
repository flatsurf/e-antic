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
void check_eq(T t, renf_class& K)
{
    renf_elem_class a;
    a = t;

    renf_elem_class b(t);

    renf_elem_class c(K);
    c = t;

    renf_elem_class d(K, t);

    #define test_neq(x,y) (x != y) || (y != x) || not (x == y) || not (y == x)
    if (test_neq(t, a)) throw 10;
    if (test_neq(t, b)) throw 10;
    if (test_neq(t, c)) throw 10;
    if (test_neq(t, d)) throw 10;
    if (test_neq(a, b)) throw 10;
    if (test_neq(a, c)) throw 10;
    if (test_neq(a, d)) throw 10;
    if (test_neq(b, c)) throw 10;
    if (test_neq(b, d)) throw 10;
    if (test_neq(c, d)) throw 10;
    #undef test_neq
}

template<typename T>
void check_not_gen(T t, renf_class& K)
{
    renf_elem_class a(K);
    renf_elem_gen(a.get_renf_elem(), K.get_renf());

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
        renf_class K(nf);
        renf_clear(nf);

        check_eq(c1, K);
        check_not_gen(c1, K);

        check_eq(c2, K);
        check_not_gen(c2, K);

        check_eq(c3, K);
        check_not_gen(c3, K);

        check_eq(c4, K);
        check_not_gen(c4, K);

        check_eq(c5, K);
        check_not_gen(c5, K);

        check_eq(c6, K);
        check_not_gen(c6, K);
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}
