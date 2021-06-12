/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_TEST_RENF_ELEM_GENERATOR_HPP
#define E_ANTIC_TEST_RENF_ELEM_GENERATOR_HPP

#include "../e-antic/renf_elem.h"

#include "renf_generator.hpp"

#include "external/catch2/single_include/catch2/catch.hpp"

namespace {

/*
 * A generator of random renf_elem_t in a given number field renf_t.
 */
struct RenfElemGenerator : public Catch::Generators::IGenerator<renf_elem_t&>
{
    flint_rand_t& state;
    renf_t& nf;
    ulong minbits, maxbits;

    mutable renf_t _nf;
    mutable renf_elem_t a;
    mutable bool has_value_for_iteration = false;
    mutable bool initial = true;

    RenfElemGenerator(flint_rand_t& state, renf_t& nf, ulong minbits, ulong maxbits) : state(state), nf(nf), minbits(minbits), maxbits(maxbits)
    {
       assert(maxbits > minbits);
    }

    void clear()
    {
        if (has_value_for_iteration) {
            renf_elem_clear(a, _nf);
            renf_clear(_nf);
        }
        has_value_for_iteration = false;
    }

    bool next() override
    {
        clear();
        return true;
    }

    renf_elem_t& get() const override
    {
        if (!has_value_for_iteration)
        {
            renf_init_set(_nf, nf);
            renf_elem_init(a, _nf);

            if (initial)
            {
                initial = false;
                renf_elem_zero(a, nf);
            }
            else
            {
                ulong bits = minbits + n_randint(state, maxbits - minbits);

                renf_elem_randtest(a, state, bits, _nf);
            }

            has_value_for_iteration = true;
        }
        return a;
    }

    ~RenfElemGenerator() override
    {
        clear();
    }
};

/*
 * Wrap RenfElemGenerator for use as GENERATE_REF(take(64, renf_elems(...))).
 */
[[maybe_unused]] Catch::Generators::GeneratorWrapper<renf_elem_t&> renf_elems(flint_rand_t& state, renf_t& nf, ulong minbits = 10, ulong maxbits = 40)
{
    return Catch::Generators::GeneratorWrapper<renf_elem_t&>(std::unique_ptr<Catch::Generators::IGenerator<renf_elem_t&>>(new RenfElemGenerator(state, nf, minbits, maxbits)));
}

}

namespace Catch {

/*
 * Make printing of `renf_elem_t` work in Catch, i.e., show meaingful debug
 * output for `CAPTURE(a)` where `a` is a `renf_elem_t`.
 * Note that it's a bit of a hack how we handle the containing renf_t here.
 * Namely, we store the current `renf_t` in a static variable so we can
 * correctly print the following `renf_elem_t`. This allows us to write:
 * ```
 * CAPTURE(nf);
 * CAPTURE(a);
 * ```
 * An alternative would have been to create a printer for something like
 * `tuple<renf_t, renf_elem_t>` and then call:
 * ```
 * CAPTURE(std::tuple(nf, a));
 * ```
 * But we want to make C programmers feel mostly at home in our
 * test code so that's probably too confusing.
 */
template <>
struct StringMaker<renf_elem_t>
{
    static std::string convert(const renf_elem_t& a)
    {
        assert(StringMaker<renf_t>::latest != nullptr && "you must always CAPTURE(nf) before capturing a contained renf_elem_t");

        char * alg = renf_elem_get_str_pretty(const_cast<renf_elem_srcptr>(a), "x", StringMaker<renf_t>::latest, 10, EANTIC_STR_ALG);
        char * emb = arb_get_str(a->emb, 10, ARB_STR_MORE);

        std::stringstream str;
        str << alg << " ~ " << emb;

        flint_free(alg);
        flint_free(emb);

        return str.str();
    }
};

}


#endif
