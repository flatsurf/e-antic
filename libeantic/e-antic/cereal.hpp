/*  This is a -*- C++ -*- header file.

    Copyright (C) 2019-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_RENFXX_CEREAL_H
#define E_ANTIC_RENFXX_CEREAL_H

#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>

#include "renf_class.hpp"
#include "renf_elem_class.hpp"

namespace eantic {
template <class Archive>
void save(Archive & archive, const boost::intrusive_ptr<const renf_class> & self)
{
   uint32_t id = archive.registerSharedPointer(self.get());

   archive(cereal::make_nvp("id", id));

   if ( id & static_cast<unsigned int>(cereal::detail::msb_32bit) )
   {
        // This is the first time cereal sees this renf_class, so we actually
        // store it. Future copies only need the id to resolve to a pointer to
        // the same renf_class.
        auto construction = self->construction();

        archive(
            cereal::make_nvp("name", std::get<1>(construction)),
            cereal::make_nvp("embedding", std::get<2>(construction)),
            cereal::make_nvp("minpoly", std::get<0>(construction)),
            cereal::make_nvp("precision", std::get<3>(construction)));
   }
}

template <class Archive>
void load(Archive & archive, boost::intrusive_ptr<const renf_class> & self)
{
    uint32_t id;
    archive(cereal::make_nvp("id", id));

    if ( id & static_cast<unsigned int>(cereal::detail::msb_32bit) )
    {
        std::string name, emb, pol;
        slong prec;

        archive(name, emb, pol, prec);

        // Restore this embedded number field.
        self = renf_class::make(pol, name, emb, prec);

        // We store a reference to this field with cereal's shared pointer machinery.
        // This shared pointer will be kept alive by cereal until the
        // deserialization of the whole file is complete. So we new an
        // intrusive pointer into this shared pointer to keep our number field
        // alive during that time.
        const std::shared_ptr<boost::intrusive_ptr<const renf_class>> shared(new boost::intrusive_ptr<const renf_class>(self));

        archive.registerSharedPointer(id, shared);
    }
    else
    {
        // Pull the number field that we stored in the `if` block out of
        // cereal's shared pointer machinery.
        self = *std::static_pointer_cast<boost::intrusive_ptr<const renf_class>>(archive.getSharedPointer(id));
    }
}

template <class Archive>
void save(Archive & archive, const renf_elem_class & self)
{
    archive(
        cereal::make_nvp("parent", boost::intrusive_ptr<const renf_class>(&self.parent())),
        cereal::make_nvp("value", self.to_string(EANTIC_STR_ALG))
    );
}

template <class Archive>
void load(Archive & archive, renf_elem_class & self)
{
    boost::intrusive_ptr<const renf_class> nf;
    std::string serialized_element;

    archive(nf, serialized_element);

    self = renf_elem_class(*nf, serialized_element);
}

}

#endif
