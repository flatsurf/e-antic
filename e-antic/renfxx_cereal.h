/*  This is a -*- C++ -*- header file.

    Copyright (C) 2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_RENFXX_CEREAL_H
#define E_ANTIC_RENFXX_CEREAL_H

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <e-antic/renfxx.h>

namespace eantic {

class renf_class_cereal {
  public:
    std::shared_ptr<const renf_class> wrapped;

  private:  
    friend cereal::access;
    template <typename Archive>
    void load(Archive & archive, std::uint32_t version)
    {
        if (version != 0) throw std::logic_error("unknown serialization from the future");

        bool rational;
        archive(rational);
        if (rational)
        {
            wrapped = nullptr;
        }
        else
        {
            std::string name, emb, pol;
            slong prec;
            
            archive(name, emb, pol, prec);

            wrapped = renf_class::make(pol, name, emb, prec);
        }
    }

    template <typename Archive>
    void save(Archive & archive, std::uint32_t) const
    {
        bool rational = !static_cast<bool>(wrapped);
        archive(cereal::make_nvp("rational", rational));
        if (rational)
        {
            return;
        }
        else
        {
            char * emb = arb_get_str(wrapped->renf_t()->emb, arf_bits(arb_midref(wrapped->renf_t()->emb)), 0);
            char * pol = fmpq_poly_get_str_pretty(wrapped->renf_t()->nf->pol, wrapped->gen_name().c_str());

            archive(
                cereal::make_nvp("name", wrapped->gen_name()),
                cereal::make_nvp("embedding", std::string(emb)),
                cereal::make_nvp("minpoly", std::string(pol)),
                cereal::make_nvp("precision", wrapped->renf_t()->prec));

            flint_free(pol);
            flint_free(emb);
        }
    }
};

template <class Archive>
void serialize(Archive &, renf_class &, std::uint32_t)
{
    static_assert(false_t<Archive>, "Cannot serialize renf_class directly with cereal as this breaks deduplication of equal fields upon deseralization. Wrap the renf_class in a renf_class_cereal instead.");
}

template <class Archive>
void save(Archive & archive, const renf_elem_class& self, std::uint32_t)
{
    archive(
        cereal::make_nvp("parent", renf_class_cereal{self.parent()}),
        cereal::make_nvp("value", boost::lexical_cast<std::string>(self)),
        cereal::make_nvp("double", static_cast<double>(self)));
}

template <class Archive>
void load(Archive & archive, renf_elem_class& self, std::uint32_t version)
{
    if (version != 0) throw std::logic_error("unknown serialization from the future");

    renf_class_cereal nf;
    std::string serialized_element;
    double _;
    
    archive(nf, serialized_element, _);

    std::stringstream ss(serialized_element);
    if (nf.wrapped)
    {
        nf.wrapped->set_pword(ss);
    }
    ss >> self;
}

}

#endif
