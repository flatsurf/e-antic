/*  This is a -*- C++ -*- header file.

    Copyright (C) 2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef RENFXX_CEREAL_H
#define RENFXX_CEREAL_H

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <e-antic/renfxx.h>

namespace eantic {

template <class Archive>
void renf_class::save(Archive & archive, std::uint32_t) const 
{
    char * emb = arb_get_str(nf->emb, arf_bits(arb_midref(nf->emb)), 0);
    char * pol = fmpq_poly_get_str_pretty(nf->nf->pol, name.c_str());

    archive(name, std::string(emb), std::string(pol), nf->prec);

    flint_free(pol);
    flint_free(emb);
}

template <class Archive>
void renf_class::load(Archive & archive, std::uint32_t version)
{
    if (version != 0) throw std::logic_error("unknown serialization from the future");

    std::string name, emb, pol;
    slong prec;
    
    archive(name, emb, pol, prec);

    *this = renf_class(pol, name, emb, prec);

    // TODO: Maybe replace with a load_and_construct so we can call a
    // deduplicating factory
}

template <class Archive>
void renf_elem_class::save(Archive & archive, std::uint32_t) const 
{
    archive(nf, boost::lexical_cast<std::string>(*this));
}

template <class Archive>
void renf_elem_class::load(Archive & archive, std::uint32_t version)
{
    if (version != 0) throw std::logic_error("unknown serialization from the future");

    std::shared_ptr<renf_class> nf;
    std::string serialized_element;
    
    archive(nf, serialized_element);

    std::stringstream ss(serialized_element);
    nf->set_pword(ss) >> *this;
}

}

#endif
