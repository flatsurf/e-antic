/*
    Copyright (C) 2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_BENCHMARK_RENFXX_NUMBER_FIELDS_HPP
#define E_ANTIC_BENCHMARK_RENFXX_NUMBER_FIELDS_HPP

#include "../../e-antic/renf_class.hpp"
#include <boost/smart_ptr/intrusive_ptr.hpp>

namespace eantic {
namespace benchmark {

/// Return a typical number field of degree `degree`.
inline auto make_number_field(size_t degree)
{
    if (degree == 1)
        return boost::intrusive_ptr<const renf_class>(&renf_class::make());
    if (degree == 2)
        return renf_class::make("x^2 - 2", "x", "1 +/- 1", 64);
    if (degree == 4)
        return renf_class::make("x^4 - 2", "x", "1 +/- 1", 64);

    throw std::logic_error("not implemented: field of this degree");
}

}
}

#endif
