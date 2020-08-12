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

#include <e-antic/renf_elem.h>

#include "../../renf/test/renf_generator.hpp"

#include "../../renfxx/test/external/catch2/single_include/catch2/catch.hpp"

namespace {

/*
 * A generator of random renf_elem_t in a given number field renf_t.
 */
struct RenfElemGenerator : public Catch::Generators::IGenerator<renf_elem_t&>
{
    flint_rand_t& state;
    renf_t& nf;
    slong iterations, minbits, maxbits;

    mutable renf_elem_t a;
    mutable slong iteration = 0;
    mutable bool has_value_for_iteration = false;

    RenfElemGenerator(flint_rand_t& state, renf_t& nf, slong iterations, slong minbits, slong maxbits) : state(state), nf(nf), iterations(iterations), minbits(minbits), maxbits(maxbits) {}

    bool next() override
    {
        if (has_value_for_iteration)
          renf_elem_clear(a, nf);
        has_value_for_iteration = false;
        return ++iteration < iterations;
    }

    renf_elem_t& get() const override
    {
        if (!has_value_for_iteration)
        {
            slong bits = minbits + static_cast<slong>(n_randint(state, maxbits - minbits));

            renf_elem_init(a, nf);
            renf_elem_randtest(a, state, bits, nf);

            has_value_for_iteration = true;
        }
        return a;
    }

    ~RenfElemGenerator()
    {
        assert(!has_value_for_iteration && "generator did not run until the end; as a result the last element can not be renf_elem_cleared since the parent renf_t has already been renf_cleared");
    }
};

/*
 * Wrap RenfElemGenerator for use as GENERATE_REF(renf_elems(...)).
 */
Catch::Generators::GeneratorWrapper<renf_elem_t&> renf_elems(flint_rand_t& state, renf_t& nf, slong iterations = 128, slong minbits = 10, slong maxbits = 40)
{
    return Catch::Generators::GeneratorWrapper<renf_elem_t&>(std::unique_ptr<Catch::Generators::IGenerator<renf_elem_t&>>(new RenfElemGenerator(state, nf, iterations, minbits, maxbits)));
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
        assert(StringMaker<renf_t>::nf != nullptr && "you must always CAPTURE(nf) before capturing a contained renf_elem_t");

        char * alg = renf_elem_get_str_pretty(const_cast<renf_elem_struct*>(a), "x", StringMaker<renf_t>::nf, 10, EANTIC_STR_ALG);
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
