/*
    Copyright (C) 2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <benchmark/benchmark.h>

#include "../../e-antic/config.h"

#include "renf_elem_class_pool.hpp"
#include "number_fields.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace eantic {
namespace benchmark {

template <typename T>
static void TrivialAddition(State& state)
{
    renf_elem_class lhs = make_number_field(state.range(0))->gen();
    T rhs = T(2);

    for (auto _ : state)
        DoNotOptimize(lhs += rhs);
}
BENCHMARK_TEMPLATE(TrivialAddition, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAddition, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAddition, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAddition, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAddition, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAddition, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);

static void TrivialReverseAddition(State& state)
{
    renf_elem_class_pool lhs(state, 2);
    renf_elem_class rhs = make_number_field(state.range(0))->gen();

    for (auto _ : state)
        DoNotOptimize(lhs() += rhs);
}
BENCHMARK(TrivialReverseAddition)->Arg(1)->Arg(2)->Arg(4);

static void RationalAddition(State& state)
{
    renf_elem_class lhs = make_number_field(state.range(0))->gen();
    renf_elem_class rhs = 2;

    for (auto _ : state)
        DoNotOptimize(lhs += rhs);
}
BENCHMARK(RationalAddition)->Arg(1)->Arg(2)->Arg(4);

template <typename T>
static void TrivialMultiplication(State& state)
{
    renf_elem_class lhs = make_number_field(state.range(0))->gen();
    T rhs = T(-1);

    for (auto _ : state)
        DoNotOptimize(lhs *= rhs);
}
BENCHMARK_TEMPLATE(TrivialMultiplication, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialMultiplication, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialMultiplication, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialMultiplication, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialMultiplication, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialMultiplication, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);

static void TrivialReverseMultiplication(State& state)
{
    renf_elem_class_pool lhs(state, -1);
    renf_elem_class rhs = make_number_field(state.range(0))->gen();

    for (auto _ : state)
        DoNotOptimize(lhs() *= rhs);
}
BENCHMARK(TrivialReverseMultiplication)->Arg(1)->Arg(2)->Arg(4);

static void RationalMultiplication(State& state)
{
    renf_elem_class lhs = make_number_field(state.range(0))->gen();
    renf_elem_class rhs = -1;

    for (auto _ : state)
        DoNotOptimize(lhs *= rhs);
}
BENCHMARK(RationalMultiplication)->Arg(1)->Arg(2)->Arg(4);

template <typename T>
static void TrivialSubtraction(State& state)
{
    renf_elem_class lhs = make_number_field(state.range(0))->gen();
    T rhs = T(2);

    for (auto _ : state)
        DoNotOptimize(lhs -= rhs);
}
BENCHMARK_TEMPLATE(TrivialSubtraction, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialSubtraction, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialSubtraction, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialSubtraction, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialSubtraction, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialSubtraction, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);

static void TrivialReverseSubtraction(State& state)
{
    renf_elem_class_pool lhs(state, 2);
    renf_elem_class rhs = make_number_field(state.range(0))->gen();

    for (auto _ : state)
        DoNotOptimize(lhs() -= rhs);
}
BENCHMARK(TrivialReverseSubtraction)->Arg(1)->Arg(2)->Arg(4);

static void RationalSubtraction(State& state)
{
    renf_elem_class lhs = make_number_field(state.range(0))->gen();
    renf_elem_class rhs = 2;

    for (auto _ : state)
        DoNotOptimize(lhs -= rhs);
}
BENCHMARK(RationalSubtraction)->Arg(1)->Arg(2)->Arg(4);

template <typename T>
static void TrivialAdditionOfProduct(State& state)
{
    renf_elem_class a;
    T b = T(2);
    renf_elem_class c = make_number_field(state.range(0))->gen();

    for (auto _ : state)
        DoNotOptimize(a += b * c);
}
BENCHMARK_TEMPLATE(TrivialAdditionOfProduct, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAdditionOfProduct, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAdditionOfProduct, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAdditionOfProduct, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAdditionOfProduct, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialAdditionOfProduct, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);

// This should be much faster than the "Trivial" invocation of += above.
template <typename T>
static void SpecializedAdditionOfProduct(State& state)
{
    renf_elem_class a;
    T b = T(2);
    renf_elem_class c = make_number_field(state.range(0))->gen();

    for (auto _ : state)
        DoNotOptimize(a.iaddmul(b, c));
}
BENCHMARK_TEMPLATE(SpecializedAdditionOfProduct, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(SpecializedAdditionOfProduct, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(SpecializedAdditionOfProduct, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(SpecializedAdditionOfProduct, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(SpecializedAdditionOfProduct, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(SpecializedAdditionOfProduct, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);

template <typename T>
static void TrivialSubtractionOfProduct(State& state)
{
    renf_elem_class a;
    T b = T(2);
    renf_elem_class c = make_number_field(state.range(0))->gen();

    for (auto _ : state)
        DoNotOptimize(a -= b * c);
}
BENCHMARK_TEMPLATE(TrivialSubtractionOfProduct, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialSubtractionOfProduct, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialSubtractionOfProduct, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialSubtractionOfProduct, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialSubtractionOfProduct, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialSubtractionOfProduct, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);

// This should be much faster than the "Trivial" invocation of -= above.
template <typename T>
static void SpecializedSubtractionOfProduct(State& state)
{
    renf_elem_class a;
    T b = T(2);
    renf_elem_class c = make_number_field(state.range(0))->gen();

    for (auto _ : state)
        DoNotOptimize(a.isubmul(b, c));
}
BENCHMARK_TEMPLATE(SpecializedSubtractionOfProduct, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(SpecializedSubtractionOfProduct, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(SpecializedSubtractionOfProduct, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(SpecializedSubtractionOfProduct, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(SpecializedSubtractionOfProduct, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(SpecializedSubtractionOfProduct, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);

template <typename T>
static void TrivialDivision(State& state)
{
    renf_elem_class lhs = make_number_field(state.range(0))->gen();
    T rhs = T(-1);

    for (auto _ : state)
        DoNotOptimize(lhs /= rhs);
}
BENCHMARK_TEMPLATE(TrivialDivision, int)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialDivision, long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialDivision, long long)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialDivision, mpz_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialDivision, mpq_class)->Arg(1)->Arg(2)->Arg(4);
BENCHMARK_TEMPLATE(TrivialDivision, renf_elem_class)->Arg(1)->Arg(2)->Arg(4);

static void TrivialReverseDivision(State& state)
{
    renf_elem_class_pool lhs(state, -1);
    renf_elem_class rhs = make_number_field(state.range(0))->one();

    for (auto _ : state)
        DoNotOptimize(lhs() /= rhs);
}
BENCHMARK(TrivialReverseDivision)->Arg(1)->Arg(2)->Arg(4);

static void RationalDivision(State& state)
{
    renf_elem_class lhs = make_number_field(state.range(0))->gen();
    renf_elem_class rhs = -1;

    for (auto _ : state)
        DoNotOptimize(lhs /= rhs);
}
BENCHMARK(RationalDivision)->Arg(1)->Arg(2)->Arg(4);

}
}
