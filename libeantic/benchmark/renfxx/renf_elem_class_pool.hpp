/*
    Copyright (C) 2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_BENCHMARK_RENFXX_RENF_ELEM_CLASS_POOL_HPP
#define E_ANTIC_BENCHMARK_RENFXX_RENF_ELEM_CLASS_POOL_HPP

#include <benchmark/benchmark.h>

#include "../../e-antic/renf_elem_class.hpp"

namespace eantic {
namespace benchmark {

/// A pool of identical renf_elem_class elments.
/// This is used in benchmarks to not distort measurements by the cost of initializing elements.
class renf_elem_class_pool
{
  public:
    renf_elem_class_pool(::benchmark::State& state, renf_elem_class prototype) : state(&state), prototype(prototype)
    {
    }

    renf_elem_class& operator()()
    {
        if (i == pool.size())
            reset();

        return pool[i++];
    }

  private:
    void reset()
    {
        state->PauseTiming();
        i = 0;
        pool = std::vector<renf_elem_class>(1024, prototype);
        state->ResumeTiming();
    }

    ::benchmark::State* state; 
    renf_elem_class prototype;
    std::vector<renf_elem_class> pool;
    size_t i = 0;
};

}
}

#endif
