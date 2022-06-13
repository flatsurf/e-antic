/*  This is a -*- C++ -*- header file.

    Copyright (C) 2019-2022 Julian Rüth

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
#include <cereal/version.hpp>
#include <cereal/types/memory.hpp>

#include "renf_class.hpp"
#include "renf_elem_class.hpp"

namespace eantic {
template <class Archive>
void save(Archive & archive, const boost::intrusive_ptr<const renf_class> & self)
{
#if CEREAL_VERSION >= 10301
    uint32_t id = archive.registerSharedPointer(std::shared_ptr<const renf_class>(self.get(), [](auto) {}));
#else
    uint32_t id = archive.registerSharedPointer(self.get());
#endif

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

namespace {

template <class Archive>
uint32_t getId(Archive& archive, ...)
{
    uint32_t id;
    archive(id);

    return id;
}

template <class Archive, typename = decltype(std::declval<Archive>().getNodeName())>
uint32_t getId(Archive& archive, int)
{
    uint32_t id;

    // Pre-1.0.0 (unreleased) versions of serialization used cereal's builtin
    // versioning. We do not use that anymore, so we have to strip it away.
    if (archive.getNodeName() == std::string{"cereal_class_version"})
      archive.finishNode();

    // Pre-1.0.0 (unreleased) version of serialization called the shared
    // pointer "shared" instead of "id" so we accept both here.
    if (std::string(archive.getNodeName()) == std::string{"shared"})
    {
      archive(cereal::make_nvp("shared", id));
    } else
    {
      archive(cereal::make_nvp("id", id));
    }

    return id;
}

}

template <class Archive>
void load(Archive & archive, boost::intrusive_ptr<const renf_class> & self)
{
    const uint32_t id = getId(archive, 0 /* ignored, needed to prefer the specialized getId over the generic one */);

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

    archive(cereal::make_nvp("parent", nf));
    archive(cereal::make_nvp("value", serialized_element));

    self = renf_elem_class(*nf, serialized_element);
}

}

#endif
