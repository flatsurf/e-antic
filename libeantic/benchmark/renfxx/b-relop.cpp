/*
    Copyright (C) 2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <benchmark/benchmark.h>

#include "../../e-antic/renfxx.h"
#include "number_fields.hpp"
#include "renf_elem_class_pool.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace eantic {
namespace benchmark {

template <typename T>
static void Equality(State& state)
{
    auto lhs = make_number_field(state.range(0))->gen();
    T rhs = T(2);

    for (auto _ : state)
        DoNotOptimize(lhs == rhs);
}
BENCHMARK_TEMPLATE(Equality, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(Equality, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(Equality, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(Equality, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(Equality, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(Equality, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);


static void ZeroEquality(State& state)
{
    auto lhs = make_number_field(state.range(0))->gen();

    for (auto _ : state)
        DoNotOptimize(lhs == 0);
}
BENCHMARK(ZeroEquality)->Arg(1)->Arg(2)->Arg(4);


static void ZeroInequality(State& state)
{
    auto lhs = make_number_field(state.range(0))->gen();

    for (auto _ : state)
        DoNotOptimize(lhs != 0);
}
BENCHMARK(ZeroInequality)->Arg(1)->Arg(2)->Arg(4);


template <typename T>
static void Inequality(State& state)
{
    auto lhs = make_number_field(state.range(0))->gen();
    T rhs = T(2);

    for (auto _ : state)
        DoNotOptimize(lhs != rhs);
}
BENCHMARK_TEMPLATE(Inequality, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(Inequality, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(Inequality, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(Inequality, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(Inequality, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(Inequality, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);

static void Nonzero(State& state)
{
    auto x = make_number_field(state.range(0))->gen();

    for (auto _ : state)
        DoNotOptimize(static_cast<bool>(x));
}
BENCHMARK(Nonzero)->Arg(1)->Arg(2)->Arg(4);

}
}

