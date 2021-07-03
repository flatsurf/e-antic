/*  This is a -*- C++ -*- header file.

    Copyright (C) 2016-2018 Vincent Delecroix
    Copyright (C) 2019-2021 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#ifndef E_ANTIC_CPPYY_H
#define E_ANTIC_CPPYY_H

#include "renfxx.h"

// On macOS/clang we get "error: cannot take the address of an rvalue of type 'std::__1::basic_ostream<char>'"
// Probably, https://bitbucket.org/wlav/cppyy/issues/95/lookup-of-friend-operator is not fixed there.
namespace eantic {
std::ostream &operator<<(std::ostream &, const renf_class &);
std::ostream &operator<<(std::ostream &, const renf_elem_class &);

namespace cppyy {

// cppyy does not see the operators provided by boost::operators so we provide
// something to make them explicit here:
template <typename S, typename T>
auto add(const S& lhs, const T& rhs) { return lhs + rhs; }
template <typename S, typename T>
auto sub(const S& lhs, const T& rhs) { return lhs - rhs; }
template <typename S, typename T>
auto mul(const S& lhs, const T& rhs) { return lhs * rhs; }
template <typename S, typename T>
auto truediv(const S& lhs, const T& rhs) { return lhs / rhs; }
template <typename T>
auto neg(const T& value) { return -value; }

inline mpq_class rational(const renf_elem_class& x) {
    return static_cast<mpq_class>(x);
}

}  // namespace cppyy
}  // namespace eantic

#endif
