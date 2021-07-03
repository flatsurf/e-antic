/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renfxx.h"

using namespace eantic;

int main(void)
{
    {
        renf_elem_class a(mpq_class("3/2"));
        if (a.floor() != 1)
            throw std::runtime_error("pb with floor");
    }
    {
        renf_elem_class a(mpq_class("-3/2"));
        if (a.floor() != floor(a))
            throw std::runtime_error("a.floor() and floor(a) disagree");
        if (a.floor() != -2)
            throw std::runtime_error("pb with floor");
    }

    return 0;
}

