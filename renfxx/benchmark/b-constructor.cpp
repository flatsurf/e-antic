/*
    Copyright (C) 2020 Vincent Delecroix
    Copyright (C) 2020 Julian Rüth

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

void ConstructTrivialField(State& state)
{
    for (auto _ : state)
    {
        DoNotOptimize(renf_class::make());
    }
}
BENCHMARK(ConstructTrivialField);

}
}
