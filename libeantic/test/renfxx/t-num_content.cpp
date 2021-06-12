/*
    Copyright (C) 2019 Vincent Delecroix
                  2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <exception>

#include "../../e-antic/renfxx.h"

using namespace eantic;

static void test_rational_content(const renf_class& K)
{
    if (renf_elem_class(K, 0).num_content() != 0)
        throw std::runtime_error("content of 0 should be 0");
    if (renf_elem_class(K, 1).num_content() != 1)
        throw std::runtime_error("content of 1 should be 1");
    if (renf_elem_class(K, -1).num_content() != 1)
        throw std::runtime_error("content of -1 should be 1");
    if (renf_elem_class(K, mpq_class(1, 3)).num_content() != 1)
        throw std::runtime_error("content of 1/3 should be 1");
    if (renf_elem_class(K, mpq_class(-2, 3)).num_content() != 2)
        throw std::runtime_error("content of -2/3 should be 2");
}

static void test_degree_one_content(const renf_class& K)
{

    if (renf_elem_class(K, "x").num_content() != 1)
        throw std::runtime_error("content of x should be 1");
    if (renf_elem_class(K, "-x").num_content() != 1)
        throw std::runtime_error("content of -x should be 1");
    if (renf_elem_class(K, "3*x").num_content() != 3)
        throw std::runtime_error("content of 3*x should be 3");
    if (renf_elem_class(K, "-3*x+6").num_content() != 3)
        throw std::runtime_error("content of -3*x+6 should be 3");
}

static void test_degree_two_content(const renf_class& K)
{
    if (renf_elem_class(K, "x^2").num_content() != 1)
        throw std::runtime_error("content of x^2 should be 1");
    if (renf_elem_class(K, "-x^2").num_content() != 1)
        throw std::runtime_error("content of -x^2 should be 1");
    if (renf_elem_class(K, "-5*x^2").num_content() != 5)
        throw std::runtime_error("content of -5*x^2 should be 5");
    if (renf_elem_class(K, "-4*x^2 + 10*x - 20").num_content() != 2)
        throw std::runtime_error("content of -4*x^2 + 10*x - 20 should be 2");
}

int main(void)
{
    {
        // renf_elem without nf
        if (renf_elem_class(0).num_content() != 0)
            throw std::runtime_error("content of 0 should be 0");
        if (renf_elem_class(1).num_content() != 1)
            throw std::runtime_error("content of 1 should be 1");
        if (renf_elem_class(mpq_class(1, 3)).num_content() != 1)
            throw std::runtime_error("content of 1/3 should be 1");
        if (renf_elem_class(mpq_class(-2, 3)).num_content() != 2)
            throw std::runtime_error("content of -2/3 should be 2");
    }

    {
        // linear
        const auto K = renf_class::make("x - 2/3", "x", "0.66 +/- 0.1");
        test_rational_content(*K);
    }

    {
        // quadratic
        const auto K = renf_class::make("x^2 - 2", "x", "1.41 +/- 0.1");
        test_rational_content(*K);
        test_degree_one_content(*K);
    }

    {
        // cubic
        const auto K = renf_class::make("x^3 - 2/5", "x", "0.74 +/- 0.1");
        test_rational_content(*K);
        test_degree_one_content(*K);
        test_degree_two_content(*K);
    }

}
