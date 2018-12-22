/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/renfxx.h"

#include <stdexcept>
#include <string>

int main(void)
{
    renf_t nf;

    renf_class K("x^3 - 2/3", "x", "0.87 +/- 0.1");

    renf_elem_class a(K, 0);
    std::string ca = "0 ~ 0.000000";
    if (ca != a.get_str())
        throw std::runtime_error("error for 0");

    renf_elem_class b(K, "x");
    std::string cb = "x ~ 0.873580";
    if (cb != b.get_str())
        throw std::runtime_error("error for x");
}
