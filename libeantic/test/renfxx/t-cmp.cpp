/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renfxx.h"

using namespace eantic;

template<typename T>
void check_eq_ne(T t, const renf_class& K)
{
    renf_elem_class a;
    a = t;

    renf_elem_class b(t);

    renf_elem_class c(K);
    c = t;

    renf_elem_class d(K, t);

    const auto L = renf_class::make(K.renf_t());
    renf_elem_class e(*L, t);

    #define test_neq(x,y) (x != y) || (y != x) || not (x == y) || not (y == x)
    if (test_neq(t, a) || test_neq(t, b) || test_neq(t, c) || test_neq(t, d) || test_neq(t, e) ||
        test_neq(a, b) || test_neq(a, c) || test_neq(a, d) || test_neq(t, e) || test_neq(b, c) ||
        test_neq(b, d) || test_neq(b, e) || test_neq(c, d) || test_neq(c, e) || test_neq(d, e))
        throw std::runtime_error("== and != not coherent on renf_elem_class (1)");
    #undef test_neq

    a += 1;
    b += 2;
    c += 3;
    d += 4;
    e += 5;
    #define test_eq(x,y) (x == y) || (y == x) || not (x != y) || not (y != x)
    if (test_eq(t, a) || test_eq(t, b) || test_eq(t, c) || test_eq(t, d) || test_eq(t, e) ||
        test_eq(a, b) || test_eq(a, c) || test_eq(a, d) || test_eq(t, e) || test_eq(b, c) ||
        test_eq(b, d) || test_eq(b, e) || test_eq(c, d) || test_eq(c, e) || test_eq(d, e))
        throw std::runtime_error("== and != not coherent on renf_elem_class (2)");
    #undef test_eq
}

template<typename T>
void check_not_gen(T t, const renf_class& K)
{
    renf_elem_class a(K);
    renf_elem_gen(a.renf_elem_t(), K.renf_t());

    if (a == t || t == a ||
        not (a != t) || not (t != a) ||
        (a < t) != (t > a) ||
        (a <= t) != (t >= a) ||
        (a > t) != (t < a) ||
        (a >= t) != (t <= a) ||
        (a < t) != not (a >= t) ||
        (a > t) != not (a <= t))
        throw std::runtime_error("comparison problem");
}

template<typename T>
void check_order(T c1, T c2, const renf_class& K)
{
    renf_elem_class a1(K, c1);
    renf_elem_class a2(K, c2);

    if (a1 == a2 || not (a1 != a2))
        throw std::runtime_error("should not be equal");

    if (a1 >= a2 || a1 > a2)
        throw std::runtime_error("a1 > a2, a1 >= a2 issue");
    if (not (a1 <= a2) || not (a1 < a2))
        throw std::runtime_error("a1 < a2, a1 <= a2 issue");
    if (a2 <= a1 || a2 < a1)
        throw std::runtime_error("a2 <= a1, a2 < a1 issue");
    if (not (a2 >= a1) || not (a2 > a1))
        throw std::runtime_error("a2 >= a1, a2 > a1 issue");
}


int main(void)
{
    int c1 = -1123;
    long c2 = 134;
    mpz_class c3(232);
    mpq_class c4(211561);
    int iter;
    FLINT_TEST_INIT(state);

    for (iter = 0; iter < 10; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 5, 64, 10);
        const auto K = renf_class::make(nf);
        renf_clear(nf);

        check_eq_ne(c1, *K);
        check_not_gen(c1, *K);

        check_eq_ne(c2, *K);
        check_not_gen(c2, *K);

        check_eq_ne(c3, *K);
        check_not_gen(c3, *K);

        check_eq_ne(c4, *K);
        check_not_gen(c4, *K);
    }

    for (iter = 0; iter < 10; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 5, 64, 10);
        const auto K = renf_class::make(nf);
        renf_clear(nf);

        check_order(-1, 1, *K);
        check_order(-1l, 13l, *K);
        check_order(mpz_class(3), mpz_class(5), *K);
        check_order(mpq_class(1,3), mpq_class(1,2), *K);
    }

    FLINT_TEST_CLEANUP(state)
    return 0;
}
