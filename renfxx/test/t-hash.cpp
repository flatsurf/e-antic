/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <e-antic/renfxx.h>

using namespace eantic;

int main(void)
{
    auto K = renf_class::make("x^2 - x - 1", "x", "1.618 +/- 0.1");
    auto L = renf_class::make("x^2 - 2", "x", "1.41 +/- 0.1");

    if (std::hash<renf_class>()(*K) == std::hash<renf_class>()(*L))
      throw std::runtime_error("hash cannot distinguish fields");

    if (std::hash<renf_elem_class>()(K->gen()) == std::hash<renf_elem_class>()(L->gen()))
      throw std::runtime_error("hash cannot distinguish elements");
    
    if (std::hash<renf_elem_class>()(mpq_class("3/2")) != std::hash<renf_elem_class>()(renf_elem_class(K, mpq_class("3/2"))))
      throw std::runtime_error("hash is not compatible with operator==");
}
