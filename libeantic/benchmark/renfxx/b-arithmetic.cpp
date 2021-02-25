/*
    Copyright (C) 2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <benchmark/benchmark.h>

#include "../../e-antic/renfxx.h"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace eantic {
namespace benchmark {

template <typename T>
static void TrivialAddition(State& state)
{
    renf_elem_class lhs(2);
    T rhs = T(2);

    for (auto _ : state)
    {
        DoNotOptimize(lhs += rhs);
    }
}
BENCHMARK_TEMPLATE(TrivialAddition, int);
BENCHMARK_TEMPLATE(TrivialAddition, long);
BENCHMARK_TEMPLATE(TrivialAddition, long long);
BENCHMARK_TEMPLATE(TrivialAddition, mpz_class);
BENCHMARK_TEMPLATE(TrivialAddition, mpq_class);
BENCHMARK_TEMPLATE(TrivialAddition, renf_elem_class);

template <typename T>
static void TrivialMultiplication(State& state)
{
    renf_elem_class lhs(2);
    T rhs = T(-1);

    for (auto _ : state)
    {
        DoNotOptimize(lhs *= rhs);
    }
}
BENCHMARK_TEMPLATE(TrivialMultiplication, int);
BENCHMARK_TEMPLATE(TrivialMultiplication, long);
BENCHMARK_TEMPLATE(TrivialMultiplication, long long);
BENCHMARK_TEMPLATE(TrivialMultiplication, mpz_class);
BENCHMARK_TEMPLATE(TrivialMultiplication, mpq_class);
BENCHMARK_TEMPLATE(TrivialMultiplication, renf_elem_class);

template <typename T>
static void TrivialSubtraction(State& state)
{
    renf_elem_class lhs(2);
    T rhs = T(2);

    for (auto _ : state)
    {
        DoNotOptimize(lhs -= rhs);
    }
}
BENCHMARK_TEMPLATE(TrivialSubtraction, int);
BENCHMARK_TEMPLATE(TrivialSubtraction, long);
BENCHMARK_TEMPLATE(TrivialSubtraction, long long);
BENCHMARK_TEMPLATE(TrivialSubtraction, mpz_class);
BENCHMARK_TEMPLATE(TrivialSubtraction, mpq_class);
BENCHMARK_TEMPLATE(TrivialSubtraction, renf_elem_class);

template <typename T>
static void TrivialDivision(State& state)
{
    renf_elem_class lhs(2);
    T rhs = T(-1);

    for (auto _ : state)
    {
        DoNotOptimize(lhs /= rhs);
    }
}
BENCHMARK_TEMPLATE(TrivialDivision, int);
BENCHMARK_TEMPLATE(TrivialDivision, long);
BENCHMARK_TEMPLATE(TrivialDivision, long long);
BENCHMARK_TEMPLATE(TrivialDivision, mpz_class);
BENCHMARK_TEMPLATE(TrivialDivision, mpq_class);
BENCHMARK_TEMPLATE(TrivialDivision, renf_elem_class);

}
}

