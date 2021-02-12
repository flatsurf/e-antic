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

#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>

#include "renfxx.h"

namespace eantic {
template <class Archive>
void save(Archive & archive, const std::shared_ptr<const renf_class> & self)
{
    uint32_t id = archive.registerSharedPointer(self.get());

    archive(cereal::make_nvp("shared", id));
    if ( id & static_cast<unsigned int>(cereal::detail::msb_32bit) )
    {
        // This is the first time cereal sees this renf_class, so we actually
        // store it. Future copies only need the id to resolve to a shared_ptr
        // to the same renf_class.
        auto construction = self->construction();

        archive(
            cereal::make_nvp("name", std::get<1>(construction)),
            cereal::make_nvp("embedding", std::get<2>(construction)),
            cereal::make_nvp("minpoly", std::get<0>(construction)),
            cereal::make_nvp("precision", std::get<3>(construction)));
    }
}

template <class Archive>
void load(Archive & archive, std::shared_ptr<const renf_class> & self)
{
    // cereal insists on creating a new instance of T when deserializing a
    // shared_ptr<T>. However this is not compatible with our uniqueness of
    // renf_class (if the field already exists, we do not create a new one but
    // just return a shared_ptr to the existing one.) Therefore, we hook into
    // cereal's deduplication logic for a compact output format but do not rely
    // on its deserialization for renf_class.
    uint32_t id;
    archive(cereal::make_nvp("shared", id));

    if ( id & static_cast<unsigned int>(cereal::detail::msb_32bit) )
    {
        std::string name, emb, pol;
        slong prec;

        archive(name, emb, pol, prec);

        self = renf_class::make(pol, name, emb, prec);

        const auto reinterpret_ptr_cast = [](const auto& ptr) noexcept
        {
            auto p = reinterpret_cast<typename std::shared_ptr<void>::element_type*>(ptr.get());
            return std::shared_ptr<void>(ptr, p);
        };

        // Register this number field so other copies in the serialization
        // resolve to it. cereal stores shared_ptr<void> internally, so we
        // need to cast away constness and then to void*.
        archive.registerSharedPointer(id,
            reinterpret_ptr_cast(
            std::const_pointer_cast<renf_class>(self)));
    }
    else
    {
        // Resolve to a copy that was previously defined in the serialization.
        // Since cereal stores that copy as a void pointer, we have to cast it
        // to a shared_ptr of the correct type first.
        self = std::static_pointer_cast<renf_class>(archive.getSharedPointer(id));
    }
}

template <class Archive>
void save(Archive & archive, const renf_elem_class & self, std::uint32_t)
{
    archive(
        cereal::make_nvp("parent", self.parent().shared_from_this()),
        cereal::make_nvp("value", self.to_string(EANTIC_STR_ALG))
    );
}

template <class Archive>
void load(Archive & archive, renf_elem_class & self, std::uint32_t version)
{
    if (version != 0) throw std::logic_error("unknown serialization from the future");

    std::shared_ptr<const renf_class> nf;
    std::string serialized_element;

    archive(nf, serialized_element);

    std::stringstream ss("(" + serialized_element + ")");
    if (nf)
        nf->set_pword(ss);
    ss >> self;
}

}

#endif
