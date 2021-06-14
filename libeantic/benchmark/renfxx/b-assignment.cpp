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
static void TrivialAssignment(State& state)
{
    auto K = make_number_field(state.range(0));
    renf_elem_class_pool lhs(state, K->zero());
    T rhs = T(2);

    for (auto _ : state)
        DoNotOptimize(lhs() = rhs);
}
BENCHMARK_TEMPLATE(TrivialAssignment, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAssignment, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAssignment, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAssignment, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAssignment, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAssignment, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);

}
}
