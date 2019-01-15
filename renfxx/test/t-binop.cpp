/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/renfxx.h"

#define CHECK_OP(a, b, K, T, OP)  \
{                                 \
    T res = a OP b;               \
{                                 \
    renf_elem_class ca(a);        \
    renf_elem_class cb(b);        \
    if (ca OP cb != res ||        \
        ca OP b != res  ||        \
        a OP cb != res)           \
        throw std::runtime_error("wrong result in binary operation"); \
}                                 \
{                                 \
    renf_elem_class ca(K, a);     \
    renf_elem_class cb(b);        \
    if (ca OP cb != res ||        \
        ca OP b != res  ||        \
        a OP cb != res)           \
        throw std::runtime_error("wrong result in binary operation"); \
}                                 \
{                                 \
    renf_elem_class ca(K);        \
    ca = a;                       \
    renf_elem_class cb(K);        \
    cb = b;                       \
    if (ca OP cb != res ||        \
        ca OP b != res ||         \
        a OP cb != res)           \
        throw std::runtime_error("wrong result in binary operation"); \
}                                 \
}

#define CHECK_DIV(a, b, K, T)     \
{                                 \
{                                 \
    renf_elem_class ca(a);        \
    renf_elem_class cb(b);        \
    if (b * (ca / cb) != a ||     \
        b * (ca / b) != a  ||     \
        b * (a / cb) != a)        \
        throw std::runtime_error("wrong result in binary operation"); \
}                                 \
{                                 \
    renf_elem_class ca(K);        \
    ca = a;                       \
    renf_elem_class cb(K);        \
    cb = b;                       \
    if (b * (ca / cb) != a ||     \
        b * (ca / b) != a ||      \
        b * (a / cb) != a)        \
        throw std::runtime_error("wrong result in binary operation"); \
}                                 \
}

template <typename T>
typename std::enable_if<std::is_unsigned<T>::value, void>::type check_binop(T a, T b, renf_class& K)
{
    CHECK_OP(a, b, K, T, +);
    CHECK_OP(a, b, K, T, *);

    if (b != 0) CHECK_DIV(a, b, K, T);
}

template <typename T>
typename std::enable_if<!std::is_unsigned<T>::value, void>::type check_binop(T a, T b, renf_class& K)
{
    CHECK_OP(a, b, K, T, +);
    CHECK_OP(a, b, K, T, -);
    CHECK_OP(a, b, K, T, *);

    if (b != 0) CHECK_DIV(a, b, K, T);
}


int main(void)
{
    int iter;
    FLINT_TEST_INIT(state);


    for (iter=0; iter<100; iter++)
    {
        renf_t nf;
        renf_randtest(nf, state, 5, 50);
        renf_class K(nf);
        renf_clear(nf);

        {
            int c1 = -1123, c2 = 142;
            check_binop(c1, c2, K);
        }
        {
            unsigned int c1 = 2223, c2 = 123;
            check_binop(c1, c2, K);
        }
        {
            long c1 = 134, c2 = -1111;
            check_binop(c1, c2, K);
        }
        {
            unsigned long c1 = 513, c2 = 3;
            check_binop(c2, c2, K);
        }
        {
            mpz_class c1(232);
            mpz_class c2(12);
            check_binop(c1, c2, K);
        }
        {
            mpq_class c1(211561);
            mpq_class c2(13);
            check_binop(c1, c2, K);
        }
    }

    {
        renf_class K1("x^2 - 2", "x", "1.41 +/- 0.01");
        renf_class K2("x^2 - 3", "x", "1.73 +/- 0.01");

        renf_elem_class a1(K1);
        renf_elem_class a2(K2);

        try
        {
            a1 + a2;
            throw std::runtime_error("a1 + a2 did not raise an error");
        }
        catch (std::domain_error)
        {
        }

        try
        {
            a1 - a2;
            throw std::runtime_error("a1 - a2 did not raise an error");
        }
        catch (std::domain_error)
        {
        }

        try
        {
            a1 * a2;
            throw std::runtime_error("a1 * a2 did not raise an error");
        }
        catch (std::domain_error)
        {
        }

        try
        {
            a1 / a2;
            throw std::runtime_error("a1 / a2 did not raise an error");
        }
        catch (std::domain_error)
        {
        }
    }

    FLINT_TEST_CLEANUP(state);
    return 0;
}

