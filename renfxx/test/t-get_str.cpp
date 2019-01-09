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

    std::string ca = "0 ~ 0.000000";
    renf_elem_class a(K, ca);
    if (ca != a.get_str())
        throw std::runtime_error("error for a=0");

    std::string cb = "x ~ 0.873580";
    renf_elem_class b(K, cb);
    if (cb != b.get_str())
        throw std::runtime_error("error for b=x");

    std::string cc = "3/7*x-2/11 ~ 0.192573";
    renf_elem_class c(K, cc);
    if (cc != c.get_str())
        throw std::runtime_error("error for c=3/7*x-2/11");
}
