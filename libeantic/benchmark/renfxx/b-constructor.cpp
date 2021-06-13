/*
    Copyright (C)      2020 Vincent Delecroix
    Copyright (C) 2020-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <benchmark/benchmark.h>

#include "../../e-antic/renfxx.h"
#include "number_fields.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace eantic {
namespace benchmark {

static void ConstructTrivialField(State& state)
{
    for (auto _ : state)
        DoNotOptimize(renf_class::make());
}
BENCHMARK(ConstructTrivialField);


static void ConstructTrivialElement(State& state)
{
    for (auto _ : state)
        DoNotOptimize(renf_elem_class());
}
BENCHMARK(ConstructTrivialElement);


template <typename T>
static void ConstructTrivialElement(State& state)
{
    T value{2};
    for (auto _ : state)
        DoNotOptimize(renf_elem_class(value)); 
}
BENCHMARK_TEMPLATE(ConstructTrivialElement, int);
BENCHMARK_TEMPLATE(ConstructTrivialElement, long);
BENCHMARK_TEMPLATE(ConstructTrivialElement, long long);
BENCHMARK_TEMPLATE(ConstructTrivialElement, mpz_class);
BENCHMARK_TEMPLATE(ConstructTrivialElement, mpq_class);
BENCHMARK_TEMPLATE(ConstructTrivialElement, renf_elem_class);


static void ConstructGenerator(State& state)
{
    auto K = make_number_field(state.range(0));

    for (auto _ : state)
        DoNotOptimize(K->gen());
}
BENCHMARK(ConstructGenerator)->Arg(1)->Arg(2)->Arg(4);


template <typename T>
static void ConstructTrivialElementInField(State& state)
{
    auto K = make_number_field(state.range(0));

    T value{};
    for (auto _ : state)
        DoNotOptimize(renf_elem_class(value)); 
}
BENCHMARK_TEMPLATE(ConstructTrivialElementInField, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(ConstructTrivialElementInField, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(ConstructTrivialElementInField, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(ConstructTrivialElementInField, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(ConstructTrivialElementInField, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(ConstructTrivialElementInField, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);


}
}
